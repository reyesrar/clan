# Sistema de Gestión de Registros del Clan

Este proyecto está diseñado para gestionar los registros históricos de un clan, digitalizarlos y automatizar la selección de sucesores para el líder del clan.

## Funcionalidades
1. Convertir datos de un archivo CSV en nodos de un arbol.
2. Imprimir la linea de sucesion actual (unicamente a los vivos).
3. Asignacion de nuevo lider en caso de muerte o vejez (Necesita Arreglo).
4. Interfaz para el usuario.

## Estructura de Carpetas
- `bin/`: Contiene el ejecutable y los archivos de datos.
- `src/`: Contiene el código fuente.

## Requisitos
- No usar bibliotecas preconstruidas de estructuras de datos dinámicas.
- Buenas prácticas de commits según las pautas.

## Observaciones
- La asignacion de un nuevo lider no esta cumpliendose automaticamente al morir o tener una edad de 70 el lider actual.