procesador lm32 usado para la asignatura de digital 2
<<<<<<< HEAD
=======
comandos:

	git init 			     | Crear un repositorio nuevo
	git pull			     | Actualizar repositorio local al commit mas nuevo
	git merge branch		     | Fusionar ramas

Pasos para modificar un repositorio a través de una rama:

<<<<<<< HEAD
	git checkout -b nombreRama           | crear y pasar a utilizar la rama nombreRama
	git add .                            | Agregar cambios
	git commit -m "cambios en mirama"    | commeter los cambios
=======
	git checkout -b nombreRama	     |crear y pasar a utilizar la rama nombreRama
	git add nombrearchivos		     | Agregar cambios
	git commit -m "mensaje"		     | commeter los cambios
>>>>>>> rama4
	git checkout master		     | Ir al master
	git merge mirama		     | Realizar cambios en rama
	git branch -d mirama		     | borrar rama
	git push origin master		     | subir al master	

>>>>>>> rama1
