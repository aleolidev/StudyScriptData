# StudyScriptData

StudyScriptData fue creado inicialmente con propósitos personales. Está enfocado a la toma de notas y el estudio en terminal, haciendo uso de comandos para navegar entre carpetas y editar archivos, y complementando dicha edición con el uso de Vim para mantener el aspecto de terminal, y no necesitar navegar entre diferentes ventanas.

La idea principal es poder tomar notas rápidamente, ya sea de ciertos conceptos, apuntes puntuales a considerar, etc y poder tenerlo todo a mano.

## Descarga y compilación

Para la descarga recurriremos a:

  `git clone https://github.com/inmortalkaktus/StudyScriptData.git`

Una vez creada la carpeta *StudyScriptData* en el destino en el que clonamos el repositorio, será necesario instalar ciertos paquetes para poder compilar el script.

  `sudo apt-get install libboost-filesystem-dev`

O en caso de error con el anterior comando.

  `sudo apt-get install libboost-all-dev`

También será necesario tener Vim instalado para un correcto funcionamiento del modo archivo.

Un vez instalados todos los paquetes necesarios, podemos proceder a compilar el archivo con el comando

  `g++ SSD.cpp -o SSD -lboost_filesystem -lboost_system -lreadline`

Y para ejecutarlo

  `./SSD`

## Funciones

Tras abrir el archivo, contamos con una serie de comandos los cuales siempre podremos consultar mediante el comando h.

### Modo carpeta

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

Aclarar que tanto en la creación como en la lectura de archivos, se trabajará en formato .ode, que viene formateado por el propio programa.

### Modo archivo

De nuevo, la explicación de cada comando

  * l: mostrar conceptos y definiciones
  * c: mostrar conceptos
  * a: editar concepto   
  * g: editar grupo de conceptos
  * m: crear nuevo concepto
  * r: eliminar concepto existente
  * o: ordenar conceptos
  * h: comandos para el modo archivo
  * e: salir del modo archivo

Y un ejemplo de ejecución (considerando que hemos abierto un archivo llamado "Tema 1.ode"):

```
Tema 1: l
- Condicional: Una sentencia condicional es una instrucción o grupo de instrucciones que se pueden ejecutar o no en función del valor de una condición.

Tema 1: c
Condicional

Tema 1: a Condicional
//Apertura del editor con Vim

Tema 1: g
//Apertura del editor con Vim, mostrando en el todos los conceptos del archivo

Tema 1: m
//Apertura del editor con Vim

Tema 1: r Concepto
Tema 1: c
No se han encontrado conceptos definidos

//Este ejemplo es suponiendo que hay más de un concepto
//Introduciremos dos valores, ambos numéricos, haciendo referencia a la posición que ocupa cada concepto en la lista de conceptos
//El primer concepto ocupa la posición 1.
//Mediante este comando cambiamos su orden (sólo se puede cambiar entre dos conceptos al mismo tiempo):
//Si el primer concepto es "Condicional", y el segundo es "Bucle", tras ejecutar este comando:
Tema 1: o 1 2
//El orden ahora será "Bucle" y después "Condicional".

Tema 1: e
Saliendo del modo archivo...

data/Programación 1: 

```

**Atención**: Para un correcto funcionamiento del programa al editar los conceptos se recomienda encarecidamente seguir dos pautas:

  * Respetar el formato "Concepto **:** Definición", los dos puntos son importantes, visto que es la referencia que toma el programa para leer cada concepto con su respectiva definición, no se pueden crear conceptos sin definición, o una definición sin conceptos.
  * No dejar saltos de línea vacíos

## Uso del editor Vim

Lo básico que necesitamos saber de Vim es que tiene dos modos; el modo comando, y el modo escritura. En el modo comando podremos escribirle al programa la acción que deseamos realizar sobre el archivo (por ejemplo guardar, salir del archivo, salir sin guardar, etc). Desde el modo escritura podremos modificar el archivo

Al abrir cualquier archivo siempre iniciamos en modo comando, que al principio puede resultar un tanto extraño porque estamos escribiendo en un editor de texto, pero no vemos lo que escribimos.

Una vez abierto el archivo podemos hacer varias cosas, entre ellas entrar al modo escritura, pulsando la tecla `i`, y salir del mismo pulsando `Esc`. Veremos que al estar en el modo escritura con Vim, en la última línea de la consola, abajo a la izquierda, nos indicará `--INSERTAR--`, lo que significa que ya podemos escribir libremente. O también podemos interactuar con el modo comando, para insertarlos, antes de nada será necesario escribir dos puntos `:`, para indicarle a Vim que estamos mandándole un comando a realizar sobre el archivo.

Aquí una serie de comandos básicos para poder editar los archivos sin problemas:

`:q` *// Salir del archivo (cuando no hayamos realizado ningún cambio)*

`:q!` *// Salir del archivo sin guardar los cambios*

`:w` *// Guardar los cambios realizados*

Podemos también combinar diferentes comandos para agilizar

`:wq` *// Para guardar y salir de Vim*

Una vez escrito nuestro comando, pulsamos intro para ejecutarlo.

Con esto, ya sabemos lo suficiente para trabajar con SSD fluidamente.

## Errores conocidos

Cuanto introducimos un comando vacío en reiteradas ocasiones, al tratar de ejecutar un comando normal posteriormente, podemos recibir un error (debido a que se guardan valores vacíos en el archivo temporal con los últimos comandos ejecutados para poder desplazarnos entre ellos con las flechas arriba y abajo y puede general conflictos).

Lo recomendable ante esta situación es usar "Ctrl+C" para salir de la ejecución del programa y volverlo a ejecutar.
