procesador lm32 usado para la asignatura de digital 2

git checkout -b mirama -             |crea y pasa a utilizar la rama mirama

Pasos para modificar un repositorio a través de una rama:

	git add .                            | Agregar cambios
	git commit -m "cambios en mirama"    | commeter los cambios
	git checkout master		     | Ir al master
	git merge mirama		     | Realizar cambios en rama
	git branch -d mirama	             | borrar rama
	git push origin master		     | suir al master	

