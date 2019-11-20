# StudyScriptData

StudyScriptData fue creado inicialmente con propósitos personales. Está enfocado a la toma de notas y el estudio en terminal, haciendo uso de comandos para navegar entre carpetas y editar archivos, y complementando dicha edición con el uso de Vim para mantener el aspecto de terminal, y no necesitar navegar entre diferentes ventanas.

La idea principal es poder tomar notas rápidamente, ya sea de ciertos conceptos, apuntes puntuales a considerar, etc y poder tenerlo todo a mano.

## Descarga y compilación

Para la descarga recurriremos a:

  `git clone https://github.com/inmortalkaktus/StudyScriptData.git`

Una vez creada la carpeta *StudyScriptData* en el destino en el que clonamos el repositorio, será necesario instalar ciertos paquetes para poder compilar el script.

  `sudo apt-get install libboost-filesystem-dev`

O

  `sudo apt-get install libboost-all-dev`
  
En caso de error con el anterior comando.

Un vez instalados todos los paquetes necesarios, podemos proceder a compilar el archivo con el comando

  `g++ SSD.cpp -o SSD -lboost_filesystem -lboost_system -lreadline`

Y

  `./SSD`
  
Para ejecutarlo.

## Funciones

Tras abrir el archivo, contamos con una serie de comandos los cuales siempre podremos consultar mediante el comando h.

### Modo archivo

A continuación, una breve explicación de lo que hace cada comando

  * d: mostrar el directorio actual
  * l: mostrar el contenido del directorio actual
  * c: descender un nivel al directorio indicado
  * u: ascender un nivel en los directorios
  * m: crear una nueva carpeta
  * t: crear un nuevo archivo o tema
  * r: borrar un archivo, carpeta o tema
  * f: trabajar con un archivo o tema
  * h: guía de comandos
  * e: salir del programa
  * i: más información

Y un ejemplo de ejecución (considerando data como el directorio en que nos encontramos por defecto):

```
data: d
Directorio actual: "/home/inmortalkaktus/Escritorio/StudyScriptData/data/"

data: l
Matemáticas 1   Programación 1   Fundamentos de los Computadores   ...

//Ambas opciones son válidas
data: c Matemáticas 1   |   c "Matemáticas 1" 
data/Matemáticas 1: u
data:

data: m Fundamentos Físicos de la Informática
data: t Notas
data: l
Matemáticas 1   Programación 1   Fundamentos de los Computadores   Fundamentos Físicos de la Informática   Notas.ode   ...

data: r Notas.ode
Estás a punto de eliminar el archivo "Notas.ode"
¿Estás seguro? (s/n): s
'data/Notas.ode' borrado

data r "Fundamentos Físicos de la Informática"
Estás a punto de eliminar la carpeta "Fundamentos Físicos de la Informática"
¿Estás seguro? (s/n): s
removed directory 'data/Fundamentos Físicos de la Informática'

data: l
Matemáticas 1   Programación 1   Fundamentos de los Computadores   ...

data: c Programación 1
data/Programación 1: l
Tema 1.ode
data/Programación 1: f Tema 1.ode
/*Se inicia el modo archivo*/
...

data: e
Vaciando archivo temporal
Saliendo...

```

## Errores conocidos

Cuanto introducimos un comando vacío en reiteradas ocasiones, al tratar de ejecutar un comando normal posteriormente, podemos recibir un error (debido a que se guardan valores vacíos en el archivo temporal con los últimos comandos ejecutados para poder desplazarnos entre ellos con las flechas arriba y abajo y puede general conflictos).

Lo recomendable ante esta situación es usar "Ctrl+C" para salir de la ejecución del programa y volverlo a ejecutar.
