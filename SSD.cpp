/* COMANDO PARA COMPILAR:
g++ SSD.cpp -o SSD -lboost_filesystem -lboost_system -lreadline
*/

/*
TO DO: Ordenar las funciones
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <boost/filesystem.hpp>
#include <readline/readline.h>
#include <readline/history.h>
using namespace std;

/*  *Formatear texto*   */

string removeLastChar(string str); //Devuelve el string pasado por argumentos sin el último carácter
string absolute_substr(int start, int end, string str); // Devuelve el string cortado desde el índice inicial hasta el final
string string_replace(string str, string initial, string replace_for); //Reemplaza un fragmento de texto por otro
string changeNforNewLines (string str);// Cambia los saltos de línea como texto a saltos de línea reales ("\\n" -> "\n")
string removeDuplicatedNewLine(string str); // Reemplaza los "\\n\\n" por "\\n"
bool haveSpaces(string str); //Comprueba si la string tiene espacios " "
bool haveSpacesAsN(string str); //Comprueba si la string tiene saltos de línea como texto ("\\n")

/*  *Readline*  */

char *character_name_generator (const char *text, int state);
static char **character_name_completion (const char *text, int start, int end);
string cinDir(string prefix);

/*  *Trabajo con rutas/directorios  */

string getExePath(); //Devuelve la ruta desde /usr/ hasta el lugar donde se está ejecutando el programa
string navigateMainTo(string add); //Haciendo uso de getExePath, devuelve la ruta desde /usr/ hasta una carpeta o archivo especificada dentro de la carpeta donde se está ejecutando el programa
string sumDirectories(string firstPath, string secondPath); //Suma de directorios (obsoleta, visto que es una simple suma de strings)
string getFileName(string str); //Devuelve el nombre del archivo de la ruta especificada

string fixInput(string str);
string getFormat(string str);
string formatFormat(string str);
string formatLine(string str);
string getTextFromFile(string str);
string getConcept(string str);
string getDefinition(string str);
string removeLastNewLine(string str);
string getConceptDefinition(string text, string concept);
string formatTemp(bool &applyChanges);
string swapConcepts(string str, int swap1, int swap2, int totalConcepts);
bool checkIfIsACommand(string list, char input);
bool existDirectory(string str);
bool existFile(string str);
bool existConcept(string text, string concept);
char getLastChar(string str);
void fileMode(string str);
void readFile(string str);
void editConcept(string str, string concept);
void editAllConcepts(string str);
void createConcept(string str);
void showConcepts(string str);
void removeConcept(string str, string concept);
void orderConcepts(string str, string concepts);
void editTemp();
void prepareEnvironment();
void createOrVoidTemp();
void printLogo();
void getCommand();
void listDirectory(string path);
void setWordsFolderElement(string path);
void setWordsTheme(string path);
int getStartIndex(char letter, string str);
int getEndIndex(char letter, string str);

//Variable global para las palabras
vector<string> wordsAutoComplete; 

int main(){

    rl_attempted_completion_function = character_name_completion;
    rl_completer_quote_characters = "\"'";

    prepareEnvironment();
    printLogo();

    getCommand();

    return 0;
}

/*  *MODO ARCHIVO*  */

void getCommand(){

    string commands = "dDlLcCuUmMtTrRfFhHeEiI";
    string command;
    char input;
    string add;
    char lastChar;
    string outPath = navigateMainTo("/");
    string insidePath = "data/";
    string path = outPath + insidePath;

    cout << sumDirectories(outPath, insidePath) << endl;

    cout << endl;

    do{
        string formattedPath;

        if(getLastChar(insidePath) == '/'){

            formattedPath = removeLastChar(insidePath);
        } else {

            formattedPath = insidePath;
        }

        setWordsFolderElement(outPath + insidePath);


        do{
            command = cinDir("\033[1m\033[34m" + formattedPath + "\033[0m: ");

            if((command[1] == ' ' || command[1] == '\0') && command[0] != ' ' && command[0] != '\t' && command[0] != '\n'){
                input = command[0];
            } else {
                input = '\0';
            }

        } while(!checkIfIsACommand(commands, input));

        while(command[0] == '\n'){
            command = string_replace(command, "\n", "");
        }

        if(command.length() >= 2){
            add = absolute_substr(2, command.length(), command);
            add = fixInput(add);
            add = string_replace(add, "\"", "");
            add = string_replace(add, "'", "");
        }

        switch (input){
            case 'd':
            case 'D':
            {
                cout << "\033[1m\033[32mDirectorio actual\033[0m: \"" << outPath << insidePath << "\"\n";
            }
            break;
            case 'l':
            case 'L':
            {
                path = outPath + insidePath;
                listDirectory(path);
            }
            break;
            case 'c':
            case 'C':
            {
                char status;
                if(getLastChar(add) != '/'){
                    add += "/";
                }

                if (!existDirectory(outPath + insidePath + add)){
                    cout << "Este directorio no existe, ¿deseas crearlo? (s/n): ";
                    cin >> status;
                    if(status == 's' || status == 'S'){
                        boost::filesystem::create_directory(sumDirectories(outPath + insidePath, add));
                        insidePath += add;
                    }
                } else {
                    insidePath += add;
                }
            }
            break;
            case 'u':
            case 'U':
            {
                if (insidePath != "data/"){
                    do{
                        insidePath = removeLastChar(insidePath);
                    }while(getLastChar(insidePath) != '/');
                }
            }
            break;
            case 'm':
            case 'M':
            {
                if(getLastChar(add) != '/'){
                    add += "/";
                }
                boost::filesystem::create_directory(sumDirectories(outPath + insidePath, add));
            }
            break;
            case 't':
            case 'T':
            {
                
                if(add.find('.') < 0 || add.find('.') > add.length()){
                    add = add + ".ode";
                }

                ofstream ofs;
                string name = sumDirectories(outPath + insidePath, add);
                if (existFile(name)) {
                    char replace;
                    cout << "El archivo " << add << " ya existe\n¿Deseas reemplazarlo por un archivo vacío? (s/n): ";
                    cin >> replace;
                    if(replace == 's' || replace == 'S'){
                        ofs.open(name.c_str());
                        ofs.close();
                    } else {
                        cout << "Abortando..." << endl;
                    }
                } else {
                    ofs.open(name.c_str());
                    ofs.close();
                }
                
            }
            break;
            case 'r':
            case 'R':
            {
                
                string name = sumDirectories(outPath + insidePath, add);
                string pathToRemove = sumDirectories(insidePath, add);
                string comando;
                char status;
                if(existFile(name)){
                    if (existDirectory(name)){
                        cout << "Estás a punto de eliminar la carpeta \"" << add << "\"\n¿Estás seguro? (s/n): ";
                        cin >> status;
                        if(status == 's' || status == 'S'){
                            if(haveSpaces(add)){
                                add = "\"" + add + "\"";
                                pathToRemove = sumDirectories(insidePath, add);
                            }
                            comando = "rm -rfv " + pathToRemove;
                            system(comando.c_str());
                        } else {
                            cout << "Abortando..." << endl;
                        }
                    } else {
                        cout << "Estás a punto de eliminar el archivo \"" << add << "\"\n¿Estás seguro? (s/n): ";
                        cin >> status;
                        if(status == 's' || status == 'S'){
                            if(haveSpaces(add)){
                                add = "\"" + add + "\"";
                                pathToRemove = sumDirectories(insidePath, add);
                            }
                            comando = "rm -v " + pathToRemove;
                            system(comando.c_str());
                        } else {
                            cout << "Abortando..." << endl;
                        }
                    }
                } else {
                    cout << "\aLa ruta especificada es inválida" << endl; 
                }
            }
            break;
            case 'f':
            case 'F':
            {
                
                string file = sumDirectories(outPath + insidePath, add);
                if(existFile(file)){
                    if(getFormat(add) == "ode"){
                        fileMode(file);
                    } else {
                        cout << "La extensión del archivo es inválida" << endl;
                    }
                } else {
                    cout << "Tema inválido" << endl;
                }
            }
            break;
            case 'h':
            case 'H':
            {
                cout << "\n   \033[1md\033[0m: mostrar el directorio actual\n   "
                "\033[1ml\033[0m: mostrar el contenido del directorio actual\n   "
                "\033[1mc\033[0m: descender un nivel al directorio indicado\n   "
                "\033[1mu\033[0m: ascender un nivel en los directorios\n   "
                "\033[1mm\033[0m: crear una nueva carpeta\n   "
                "\033[1mt\033[0m: crear un nuevo archivo o tema\n   "
                "\033[1mr\033[0m: borrar un archivo, carpeta o tema\n   "
                "\033[1mf\033[0m: trabajar con un archivo o tema\n   " //formato de los temas: .ode
                "\033[1mh\033[0m: guía de comandos\n   "
                "\033[1me\033[0m: salir del programa\n   "
                "\033[1mi\033[0m: más información\n"
                << endl;
            }    
            break;
            case 'e':
            case 'E':
            {
                cout << "Vaciando archivo temporal" << endl;
                createOrVoidTemp();
                cout << "Saliendo..." << endl;
            }    
            break;
            case 'i':
            case 'I':
            {
                cout << "\n"
                "\033[1m\033[32mPrograma creado por aaleexom\033[0m\n\n"
                "La finalidad de SSD(\033[1mStudyScriptData\033[0m) es ayudar a aquellos estudiantes amantes del software y que se sienten cómodos trabajando en una terminal para poder organizarse sus apuntes, tomar notas e incluso estudiar en él.\n\n";
            }
            break;
        }
    }while(input != 'e' && input != 'E');
}

/*  *MODO TEMA*  */

void fileMode(string str) {

    char input;
    string commands = "lLcCaAgGmMrRoOhHeE";
    string command;
    string fileName = absolute_substr(0, getFileName(str).find('.') , getFileName(str));
    string add;

    cout << endl;

    do{ 

        setWordsTheme(str);

        do{
            command = cinDir("   \033[1m\033[32m" + fileName + "\033[0m: ");

            if((command[1] == ' ' || command[1] == '\0') && command[0] != ' ' && command[0] != '\t' && command[0] != '\n'){
                input = command[0];
            } else {
                input = '\0';
            }

        } while(!checkIfIsACommand(commands, input));

        if(command.length() >= 2){
            add = absolute_substr(2, command.length(), command);
            add = fixInput(add);
            add = string_replace(add, "\"", "");
            add = string_replace(add, "'", "");
        }

        switch(input){
            case 'l':
            case 'L':
            {
                readFile(str);
            }
            break;
            case 'c':
            case 'C':
            {
                showConcepts(str);
            }
            break;
            case 'a':
            case 'A':
            {
                editConcept(str, add);
            }
            break;
            case 'g':
            case 'G':
            {
                editAllConcepts(str);
            }
            break;
            case 'm':
            case 'M':
            {
                createConcept(str);
            }
            break;
            case 'r':
            case 'R':
            {
                removeConcept(str, add);
            }
            break;
            case 'o':
            case 'O':
            {
                orderConcepts(str, add);
            }
            break;
            case 'h':
            case 'H':
            {
                cout << endl <<
                "      \033[1ml\033[0m: mostrar conceptos y definiciones\n"
                "      \033[1mc\033[0m: mostrar conceptos\n"
                "      \033[1ma\033[0m: editar concepto\n"
                "      \033[1mg\033[0m: editar grupo de conceptos\n"
                "      \033[1mm\033[0m: crear nuevo concepto\n"
                "      \033[1mr\033[0m: eliminar concepto existente\n"
                "      \033[1mo\033[0m: ordenar conceptos\n"
                "      \033[1mh\033[0m: comandos para el modo archivo\n"
                "      \033[1me\033[0m: salir del modo archivo\n"
                << endl;
            }
            break;
        }
    }while(input != 'e' && input != 'E');

    cout << "   Saliendo del modo archivo...\n\n";
}

string cinDir(string prefix){

    char *buf;
    string returnResult;

    buf = readline(prefix.c_str());
    returnResult = buf;

    if (buf[0] == '\0')
    {
        free (buf);
    }
    else
    {
        add_history (buf);
    }

    free (buf);
    buf = NULL;

    return returnResult;
}

string getExePath(){

    string path = boost::filesystem::current_path().string();

    return path;
}

string navigateMainTo(string add){
    string path = sumDirectories(getExePath(), add);

    return path;
}

string sumDirectories(string firstPath, string secondPath){

    string path = firstPath + secondPath;

    return path;
}

string formatFormat(string str){

    string formattedString = str;

    if (getStartIndex('.', str) == -1){
        formattedString = str + ".ode";
    }

    return formattedString;
}

string getFormat(string str){

    string filename;
    int i = str.length() - 1;
    int j = 0;

    do{
        i--;
        j++;
    }while(str[i] != '.' && i >= 0);

    filename = str.substr(i + 1, j);

    return filename;
}

string formatLine(string str){

    string concept, definition, formattedLine;

    concept = getConcept(str);
    definition = getDefinition(str);

    formattedLine = "\033[1m" + concept + "\033[0m: " + definition + "\n";

    return formattedLine; 
}

string getTextFromFile(string str){
    string line;
    string text = "";

    ifstream file(str);
    ofstream tempofs;
    ofstream originalofs;

    if (file.is_open())
    {
        while (getline(file,line))
        {
            text += line + "\\n";
        }
        text = removeLastNewLine(text);
    } else {
        cout << "No se puede abrir el archivo";
    }

    return text;
}

string getConcept(string str){

    int startIndex = getStartIndex('$', str) + 1;
    int endIndex = getStartIndex('|', str);

    string concept = absolute_substr(startIndex, endIndex, str);

    return concept;

}

string getDefinition(string str){

    int startIndex = getStartIndex('|', str) + 1;
    int endIndex = getEndIndex('$', str);

    string definition = absolute_substr(startIndex, endIndex, str);

    return definition;
}

string absolute_substr(int start, int end, string str){

    int real_end = end - start;
    string realSubstr = str.substr(start, real_end);

    return realSubstr;
}

string removeLastNewLine(string str){

    string result;
    int length = str.length() - 1;

    if(str[length] == 'n' && str[length - 1] == '\\'){
        result = absolute_substr(0, length - 1, str);
    } else {
        result = str;
    }

    return result;
}

string getConceptDefinition(string text, string concept){

    string definition;
    int start = text.find('|', text.find("$" + concept + "|")) + 1;
    int end = text.find('$', start);

    definition = absolute_substr(start, end, text);

    return definition;
}

string formatTemp(bool &applyChanges){

    string line;
    string tempText = "";
    string concept;
    string definition;
    int doubledotindex = 0;

    ifstream temp(navigateMainTo("/data/.temp"));
    if (temp.is_open())
    {
        while (getline(temp,line))
        {
            tempText += "$" + line + "$\\n";
        }
        temp.close();
    } else {
        cout << "No se ha encontrado el archivo .temp" << endl;
    }

    tempText = removeLastNewLine(tempText);

    if(tempText.find(':') >= 0 && tempText.find(':') < tempText.length()){
        applyChanges = true;
        tempText = string_replace(tempText, ":", "|");
        tempText = string_replace(tempText, "| ", "|");
    } else {
        applyChanges = false;
    }

    return tempText;
}

string changeNforNewLines(string str){

    string result = string_replace(str, "\\n", "\n");

    return result;
}

string string_replace(string str, string initial, string replace_for){

    string result = "";
    int index = 0;
    int index2 = str.find(initial, index);

    while(index >= 0 && index < str.length()){
        if (str.find(initial, index) >= 0 && str.find(initial, index) < str.length()){
            result += absolute_substr(index, index2, str) + replace_for;
            index = index2 + initial.length();
            index2 = str.find(initial, index);
        } else {
            result += absolute_substr(index, str.length(), str);
            index = str.find(initial, index);
        }
        
    }

    return result;
}

string swapConcepts(string str, int swap1, int swap2, int totalConcepts){

    string firstOriginalCode;
    string middleOriginalCode;
    string finalOriginalCode;

    string firstSwap;
    string secondSwap;

    string text = getTextFromFile(str);
    string swappedText;

    int i;
    int start = 0;
    int end = 0;

    for(i = 0; i < (swap1 - 1); i++){
        end = text.find('$', end);
        end = text.find('$', end + 1) + 3;
    }

    firstOriginalCode = absolute_substr(0, end, text);

    start = 0;
    end = 0;

    for(i = 0; i < swap1; i++){
        start = text.find('$', end);
        end = text.find('$', start + 1) + 3;
    }

    firstSwap = absolute_substr(start, end, text);

    start = 0;
    end = 0;

    for(i = 0; i < (swap2 - 1); i++){
        if(i < swap1){
            start = text.find('$', start);
            start = text.find('$', start + 1) + 3;
        }
        if(i < swap1){
            end = start;
        } else {
            end = text.find('$', end);
            end = text.find('$', end + 1) + 3;
        }
    }

    middleOriginalCode = absolute_substr(start, end, text);

    start = 0;
    end = 0;

    for(i = 0; i < swap2; i++){
        start = text.find('$', end);
        end = text.find('$', start + 1) + 3;
    }

    secondSwap = absolute_substr(start, end, text);

    if (!haveSpacesAsN(secondSwap)){
        secondSwap = secondSwap + "\\n";
    }

    start = 0;
    end = 0;

    for(i = 0; i < totalConcepts; i++){
        if(i < swap2){
            start = text.find('$', start);
            start = text.find('$', start + 1) + 3;
        }
        if(i < swap2){
            end = start;
        } else {
            end = text.length();
        }
    }

    if(start < 0 || start >= text.length()){
        start = text.length();
    }
    if(end < 0 || end > text.length()){
        end = text.length();
    }

    finalOriginalCode = absolute_substr(start, end, text);

    if(finalOriginalCode == ""){
        firstSwap = string_replace(firstSwap, "\\n", "");
    }

    swappedText = firstOriginalCode + secondSwap + middleOriginalCode + firstSwap + finalOriginalCode;

    return swappedText;
}

int getStartIndex(char letter, string str){

    int index = 0;

    while(str[index] != letter && index < str.length()){
        index++;
    }

    if(index >= str.length()){
        index = -1;
    }
    
    return index;
}

int getEndIndex(char letter, string str){

    int index = str.length() - 1;

    while(str[index] != letter && index >= 0){
        index--;
    }
    
    return index;
}

void readFile(string str){

    string text = "";
    string line;
    ifstream file(str);
    if (file.is_open())
    {
        cout << endl;
        while (getline(file,line))
        {
            text += "   - " + formatLine(line) + "\n";
        }
        file.close();

        if(text != ""){
            cout << text;
        } else {
            cout << "\033[1m\033[31m   No se han encontrado conceptos definidos\033[0m\n\n";
        }
    } else {
        cout << "No se puede abrir el archivo";
    } 
}

void editConcept(string str, string concept){

    string definition;
    string tempText;
    string resultText;
    string text = getTextFromFile(str);

    int start = 0;
    int end = 0;

    bool applyChanges = false;

    ofstream tempofs;
    ofstream originalofs;

    createOrVoidTemp();

    if(existConcept(text, concept)){
        definition = getConceptDefinition(text, concept);
        tempText = concept + ":" + definition;

        start = text.find("$" + concept + "|");
        end = text.find('$', start + 1) + 1;

        tempofs.open(navigateMainTo("/data/.temp"), std::ios_base::app);
        tempofs << tempText;
        tempofs.close();
        editTemp();

        tempText = formatTemp(applyChanges);

        resultText = absolute_substr(0, start, text) + tempText + absolute_substr(end, text.length(), text);
        resultText = changeNforNewLines(resultText);
        resultText = removeDuplicatedNewLine(resultText);

        if(applyChanges){
            originalofs.open(str);
            originalofs << resultText;
            originalofs.close();
        } else {
            cout << "   El formato del concepto es inválido.\n\n";
        }
    }
}

void editAllConcepts(string str){
    string text = getTextFromFile(str);
    string tempText;

    bool applyChanges = false;

    text = string_replace(text, "$", "");
    text = string_replace(text, "|", ": ");
    text = string_replace(text, "\\n", "\n");

    ofstream tempofs;
    ofstream originalofs;

    createOrVoidTemp();

    tempofs.open(navigateMainTo("/data/.temp"), ios_base::app);
    tempofs << text;
    tempofs.close();
    editTemp();

    tempText = formatTemp(applyChanges);
    tempText = changeNforNewLines(tempText);
    tempText = removeDuplicatedNewLine(tempText);

    if(applyChanges){
        originalofs.open(str);
        originalofs << tempText;
        originalofs.close();
    } else {
        cout << "   El formato del concepto es inválido.\n\n";
    }
}

void createConcept(string str){

    string tempText;

    bool applyChanges = false;

    ofstream tempofs;
    ofstream originalofs;

    createOrVoidTemp();

    tempofs.open(navigateMainTo("/data/.temp"), ios_base::app);
    tempofs << "Concept:Definition";
    tempofs.close();
    editTemp();

    tempText = formatTemp(applyChanges);
    tempText = changeNforNewLines(tempText);
    tempText = "\n" + tempText;
    tempText = removeDuplicatedNewLine(tempText);

    if(tempText == "$Concept|Definition$"){
        applyChanges = false;
        cout << "   Cancelando creación de concepto.\n\n";
    } else {
        if(applyChanges){
            originalofs.open(str, ios_base::app);
            originalofs << tempText;
            originalofs.close();
        } else {
            cout << "   El formato del concepto es inválido.\n\n";
            tempofs.open(navigateMainTo("/data/.temp"), ios_base::app);
            tempofs.close();
            tempText = formatTemp(applyChanges);
            tempText = changeNforNewLines(tempText);
        }
    }
}

void showConcepts(string str){

    ifstream file(str);
    string text = getTextFromFile(str);

    int index = 0;
    int index2 = 0;
    int maxLength;

    maxLength = text.length();
    
    cout << "\n";

    while (index < maxLength && index2 < maxLength){
        index = text.find('$', index) + 1;
        index2 = text.find('|', index2);

        cout << "\033[1m" << absolute_substr(index, index2, text) << "\033[0m    ";

        index = text.find('$', index) + 1;
        index2++;
    }

    if(text == ""){
        cout << "\033[1m\033[31m   No se han encontrado conceptos definidos\033[0m";
    }

    cout << "\n\n";
}

void removeConcept(string str, string concept){

    string line;
    string text = getTextFromFile(str);

    int start = 0;
    int end = 0;

    ifstream file(str);
    ofstream fileofs;

    if(existConcept(text, concept)){
        start = text.find("$" + concept + "|");
        end = text.find("$", text.find ("|", start + 1)) + 3;
        if(end > text.length()){
            end = text.length();
        }
        text = absolute_substr(0, start, text) + absolute_substr(end, text.length(), text);

        text = removeLastNewLine(text);
        text = string_replace(text, "\\n", "\n");
        text = removeDuplicatedNewLine(text);

        fileofs.open(str);
        fileofs << text;
        fileofs.close();
        cout << "      " << concept << " eliminado" << endl;
    } else {
        cout << "      Concepto inválido\n\n";
    }


}

void orderConcepts(string str, string concepts){
    
    ifstream file(str);
    string text = getTextFromFile(str);
    string orderedText;
    string newOrder;
    string line;

    bool allNumbers = true;

    char input;
    
    int swap1 = 0;
    int swap2 = 0;
    string swap1s;
    string swap2s;

    int index = 0;
    int index2 = 0;
    int i = 1;
    int maxLength = text.length();;

    int conceptsAmount = 0;

    for(index = 0; index <= maxLength; index++){
        if(text.find('|', index) >= 0 && text.find('|', index) < maxLength){
            index = text.find('|', index) + 1;
            conceptsAmount++;
        }
    }

    ofstream fileofs;

    if(conceptsAmount > 1){
        if(concepts.find(' ') > 0 && concepts.find(' ') < concepts.length()){

            swap1s = absolute_substr(0, concepts.find(' '), concepts);
            swap2s = absolute_substr(concepts.find(' '), concepts.length(), concepts);

            while(swap1s.find(' ') > 0 && swap1s.find(' ') < swap1s.length()){
                swap1s = string_replace(swap1s, " ", "");
            }
            while(swap2s.find(' ') > 0 && swap2s.find(' ') < swap2s.length()){
                swap2s = string_replace(swap2s, " ", "");
            }

            swap1 = stoi(swap1s);
            swap2 = stoi(swap2s);

            if(swap1 > 0 && swap1 <= conceptsAmount && swap2 > 0 && swap2 <= conceptsAmount){
                orderedText = swapConcepts (str, swap1, swap2, conceptsAmount);
                orderedText = changeNforNewLines(orderedText);
                orderedText = removeDuplicatedNewLine(orderedText);

                fileofs.open(str);
                fileofs << orderedText;
                fileofs.close();
            } else{
                cout << "   Valores inválidos" << endl;
            }

            cout << endl;
        } else {
            cout << "   Valores inválidos" << endl;
        }

    } else {
        cout << "   No hay suficientes conceptos para reordenar" << endl;
    }
}

void editTemp(){

    string command = "vim data/.temp";

    system(command.c_str());

}

bool checkIfIsACommand(string list, char input){
    
    int i = 0;
    bool status = false;

    for(i = 0; i < list.length(); i ++){
        if(list[i] == input){
            status = true;
        }
    }

    return status;
}

bool haveSpaces(string str){
    int strLength = str.length();
    int i;
    bool haveSpaces = false;

    for(i = 0; i < strLength; i++){
        if(str[i] == ' '){
            haveSpaces = true;
        }
    }

    return haveSpaces;
}

bool haveSpacesAsN(string str){
    
    int i;
    bool haveSpaces = false;

    if(str.find("\\n") >= 0 && str.find("\\n") < str.length()){
        haveSpaces = true;
    }

    return haveSpaces;
}


bool existDirectory(string str){
    if(boost::filesystem::is_directory(str)){
        return true;
    } else {
        return false;
    }
}
bool existFile(string str){

    struct stat buffer;

    if (stat (str.c_str(), &buffer) == 0) {
        return true;
    } else {
        return false;
    }
}

bool existConcept(string text, string concept){

    string toSearch = "$" + concept + "|";
    bool exist = false;

    if(text.find(toSearch) >= 0 && text.find(toSearch) < text.length()){
        exist = true;
    }

    return exist;
}

string removeLastChar(string str){
    string finalString = str.substr(0, str.length() - 1);

    return finalString;
}

char getLastChar(string str){
    char lastChar = str[str.length() - 1];

    return lastChar;
}

string getFileName(string str){

    string filename;
    int i = str.length() - 1;
    int j = 0;

    do{
        i--;
        j++;
    }while(str[i] != '/');

    filename = str.substr(i + 1, j);

    return filename;
}

string fixInput(string str){

    int strLength = str.length();
    int i;
    bool firstLetterFound = false;
    string formattedString = "";

    for(i = 0; i < strLength; i++){
        if(!firstLetterFound && str[i] != ' '){
            firstLetterFound = true;
        }
        if(firstLetterFound){
            formattedString += str[i];
        }
    }

    return formattedString;
    cout << "formattedString: " << formattedString << endl;
}

string removeDuplicatedNewLine(string str){

    string formattedString = string_replace(string_replace(str, "\\n\\n", "\\n"), "\n\n", "\n");
    while(formattedString[0] == '\n'){
        formattedString = absolute_substr(1, formattedString.length(), formattedString);
    }
    while(formattedString[formattedString.length() - 1] == '\n'){
        formattedString = absolute_substr(0, formattedString.length() - 1, formattedString);
    }
    formattedString = string_replace(formattedString, "$$\n", "");
    formattedString = string_replace(formattedString, "$$", "");
    while(formattedString[0] == '\n'){
        formattedString = absolute_substr(1, formattedString.length(), formattedString);
    }
    while(formattedString[formattedString.length() - 1] == '\n'){
        formattedString = absolute_substr(0, formattedString.length() - 1, formattedString);
    }
    return formattedString;
}

void listDirectory(string path){

    int objCounter = 0;

    boost::filesystem::directory_iterator end_itr;

    for (boost::filesystem::directory_iterator itr(path); itr != end_itr; ++itr)
    {
        if (is_regular_file(itr->path())) {
            string current_file = itr->path().string();
            current_file = getFileName(current_file);
            cout << "\033[1m\033[32m" << current_file << "   ";
            objCounter++;
        } else {
            string current_folder = itr->path().string();
            current_folder = getFileName(current_folder);
            cout << "\033[1m\033[34m" << current_folder << "   ";
            objCounter++;
        }
    }

    if(objCounter == 0){
        cout << "\033[1m\033[31mNo se han encontrado carpetas o archivos";
    }

    cout << "\033[0m\n";
}

void setWordsFolderElement(string path){

    boost::filesystem::directory_iterator end_itr;
    wordsAutoComplete.clear();

    for (boost::filesystem::directory_iterator itr(path); itr != end_itr; ++itr)
    {
        string current_file = itr->path().string();
        current_file = getFileName(current_file);
        wordsAutoComplete.push_back(current_file);
    }
}

void setWordsTheme(string path){

    string formattedConcept, line;
    ifstream file(path);
    if (file.is_open()){

        wordsAutoComplete.clear();

        while (getline(file,line))
        {
            if(line.find('|') >= 0 && line.find('|') <= line.length()){
                formattedConcept = absolute_substr(0, line.find('|'), line);
                while(formattedConcept.find('|') >= 0 && formattedConcept.find('|') <= formattedConcept.length()){
                    formattedConcept = string_replace(formattedConcept, "|", "");
                }
                while(formattedConcept.find('$') >= 0 && formattedConcept.find('$') <= formattedConcept.length()){
                    formattedConcept = string_replace(formattedConcept, "$", "");
                }
                wordsAutoComplete.push_back(formattedConcept);
            }
        }
    }
}

void prepareEnvironment(){

    boost::filesystem::create_directory(sumDirectories(getExePath(), "/data/"));
    createOrVoidTemp();
}

void createOrVoidTemp(){

    ofstream temp;
    string fileAddress = navigateMainTo("/data/") + ".temp";

    temp.open(fileAddress.c_str());
    temp.close();
}

void printLogo(){
    cout <<
    "\n\n"
    "███████╗███████╗██████╗\n" 
    "██╔════╝██╔════╝██╔══██╗\n"
    "███████╗███████╗██║  ██║\n"
    "╚════██║╚════██║██║  ██║\n"
    "███████║███████║██████╔╝\n"
    "╚══════╝╚══════╝╚═════╝"
    "\n\n"
    "(\033[1m\033[32mh\033[0m) para ayuda, (\033[1m\033[31me\033[0m) para salir\n\n";
}

//Función generada para completar palabras
char *character_name_generator (const char *text, int state)
{
    int i = 0;

    static int list_index, len;
    const char *name;
    const char* words[wordsAutoComplete.size() - 1];

    for(i = 0; i < wordsAutoComplete.size(); i++){
        words[i] = wordsAutoComplete.at(i).c_str(); 
    }

    if (!state)
    {
        list_index = 0;
        len = string(text).length();
    }

    while (list_index < wordsAutoComplete.size())
    {
        name = words[list_index];
        list_index++;
        if (strncmp(name, text, len) == 0){ 
            return strdup(name);
        }
    }

    // Si no hay coincidencias, devolver NULL
    return ((char *) NULL);
}

// Función de completación
static char **character_name_completion (const char *text, int start, int end)
{
    // (Previene que haya un espacio al final del texto)
    rl_completion_append_character = '\0';
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, &character_name_generator);
}