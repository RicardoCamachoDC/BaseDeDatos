/* Ejercicio 4. Escribir un programa en C que permita realizar las siguientes consultas
SELECT a la base de datos de esta leccion devolviendo los resultados. Con este programa
deberá obtenerse:
-Las ubicaciones de los empleados segun el proyecto en el que trabajen.
-El total de numero de horas de trabajo de cada departamento.  */
 
 //Autor: Ricardo Camacho Diaz-Cano
 
 //Librerias.
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "sqlite3.h"

//Cabeceras.h
//Variables globales.
/*Se definen variables globales para mayor comodidad al trabajar con las diferentes 
funciones */
sqlite3 *db;
FILE *fich;
int res;
int opc;
char *error=0;
char *sql;
char *baseDatos;
char *fichero;
char *escCon;

//Cabeceras: 
void menu();
void abrirBase(char *BaseDatos);
void ejecucionInstruccion(char *sql, int SelectCb);
void escribirConsulta(int opc);
static int selectOpc1(void *nada, int argc, char **argv, char **colNames);
static int selectFichOpc1(void *nada, int argc, char **argv, char **colNames);
static int selectOpc2(void *nada, int argc, char **argv, char **colNames);
static int selectFichOpc2(void *nada, int argc, char **argv, char **colNames);


//Funcion Main.
int main() {
	menu();
	
	return 0;
}

//Rutinas.
/* Se crea una funcion menu, donde se indica que realiza en cada paso. */
void menu(){
	// Se imprime un mensaje de bienvenida.
	printf("\t\t -. Bienvenido .-\n");
	/* Se especifica el nombre del fichero y se abre, si se devuelve un error se 
	vuelve a pedir que se introduzca otro nombre. */
	do{
		printf("\nIntroduzca la base da datos a usar: ");
		scanf("%s", &baseDatos);
		abrirBase(&baseDatos);	
	}while(res);
	printf("\n\n");
	system("pause");
	
	do{
		/* Se limpia la pantalla y se muestra un menu de consultas con las diferentes
		opciones de consulta.*/
		system("cls");
		printf("\t\t-. Menu Consultas .-\n");
		printf("Opciones: \n");
		printf("1. Ubicaciones de los empleados segun el proyecto. \n");
		printf("2. Total de numero de horas de trabajo por departamento. \n");
		printf("3. Salir. \n");
		printf("Seleccione una opcion: ");
		scanf("%d",&opc);
		
		/*En las dos opciones se ordena los elementos conforme a la primera columna, para 
		mayor comodidad para buscar datos, en el primer caso segun el nombre del empleado 
		y para el segundo segun el nombre del departamento. */
		switch(opc){
			case 1:{
				/* Se limpia la pantalla y se busca la instruccion indicada y se muestra por
				pantalla por defecto y se llama a la funcion escribirconsulta(). */
				system("cls");
				printf("El empleado:\t\tTrabaja en:\t\tAsociado al Proy N%c:\n",167);
				sql="SELECT Empleado.Nombre_Pila,Empleado.Apellido_1,Proyecto.Proy_Ubicacion,Proyecto.Proy_Numero FROM Proyecto,Empleado, Trabaja_En WHERE Proyecto.Numero_Dpto=Empleado.Numero_Dpto AND Empleado.ID_Empleado=Trabaja_En.ID_Empleado AND Trabaja_En.Proy_Numero=Proyecto.Proy_Numero ORDER BY Empleado.Nombre_Pila;";
				ejecucionInstruccion(sql, selectOpc1);
				printf("\n\n");
				escribirConsulta(opc);
				printf("\n\n");
				system("pause");	
			}break;
			case 2:{
				/* Se limpia la pantalla y se busca la instruccion indicada y se muestra por
				pantalla por defecto y se llama a la funcion escribirconsulta(). */
				system("cls");
				printf("El Departamento:\tTiene un total de horas de trabajo:\n");
				sql="SELECT Departamento.Nombre_Dpto,sum(Trabaja_En.Horas) FROM Proyecto, Departamento, Trabaja_En WHERE Departamento.Num_Dpto=Proyecto.Numero_Dpto AND Proyecto.Proy_Numero=Trabaja_En.Proy_Numero GROUP BY Departamento.Num_Dpto ORDER BY Departamento.Nombre_Dpto;";
				ejecucionInstruccion(sql, selectOpc2);
				printf("\n\n");
				escribirConsulta(opc);
				printf("\n\n");
				system("pause");	
			}break;
			case 3:{
				/* Se limpiar la pantalla y se cierra la base de datos. */
				system("cls");
				sqlite3_close(db);	
				printf("Gracias. \n\n");
			}break;
			default:{
				/* Si no se selecciona una opcion correcta se vuelve a pedir la opcion.*/
				printf("\nSeleccione una opcion valida. ");
				printf("\n\n");
				system("pause");	
			}break;
		}	
	}while(opc!=3);
	return;
}


/* Las funciones SelectOpc muestran por pantalla el resultado de las consultas en el fomrato
especificado mientras que las funciones SelectFichOpc escriben en un fichero el resultado de 
las consultas. */
static int selectOpc1(void *nada, int argc, char **argv, char **colNames){
	printf("%s, %s\t\t%s    \t\t%s\n",argv[0],argv[1],argv[2],argv[3]);
	return 0;
}
static int selectFichOpc1(void *nada, int argc, char **argv, char **colNames){
	fprintf(fich,"%s, %s\t\t%s    \t\t%s\n",argv[0],argv[1],argv[2],argv[3]);
	return 0;
}
static int selectOpc2(void *nada, int argc, char **argv, char **colNames){	
	printf("%s \t\t%s\n",argv[0],argv[1]);
	return 0;	
}
static int selectFichOpc2(void *nada, int argc, char **argv, char **colNames){	
	fprintf(fich,"%s \t\t%s\n",argv[0],argv[1]);
	return 0;	
}

/* AbrirBase, abre la base de datos que le especifica el ususario e indica si al 
abrirla se produce algun error. */
void abrirBase(char *baseDatos){
	res=sqlite3_open(baseDatos,&db);
	if (res){
		fprintf(stderr,"No se puede abrir la base de datos: %s\n",sqlite3_errmsg(db));
	}
	else{
		fprintf(stderr, "Base de datos OK\n");
	}
	return;
}

/* ejecicionInstruccion busca la instruccion en la base de datos, y mediante selectCb
se le indica si el resultado se le imprime por pantalla o se escribe en un fichero. */
void ejecucionInstruccion(char *sql, int selectCb){
	res=sqlite3_exec(db,sql,selectCb,0,&error);
	if (res!=SQLITE_OK){
		fprintf(stderr,"Error: %s\n", error);
		sqlite3_free(error);
	}
	return;
}

/* escribirConsulta se le indica si se quiere escribir el resultado en un fichero,
si es asi, se introduce el nombre del fichero y se crea o sobreescribe, se escribe
la consulta y se cierra. En todas las operaciones se realiza un seguimiento para 
ver si la operacion se ha realizado correctamente. */
void escribirConsulta(int opc){
	printf("Desea escribir el resultado en un fichero: \n");
	printf("Si: 1 \n");
	printf("No: 0 \n");
	do{
		scanf("%s",&escCon);
	}while(escCon!='1' && escCon!='0');
	
	
	if (escCon=='1'){
		do{
			printf("\nIntroduzca el nombre del fichero: ");
			scanf("%s",&fichero);
			fich=fopen(&fichero,"a");
			if (fich ==NULL){
				printf("No se ha podido abrir.\n");
			}
		}while (fich==NULL);
		
		if (opc==1){
			ejecucionInstruccion(sql,selectFichOpc1);
			printf("\nGuardado Correctamente.\n");
		}
		else if(opc==2){
			ejecucionInstruccion(sql,selectFichOpc2);
			printf("\nGuardado Correctamente.\n");
		}
		
		fclose(fich);
	}
	return;
}
