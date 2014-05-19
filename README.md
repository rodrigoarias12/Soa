SistemasOperativos
==================
Ayuda para usar el git:

git status                    -> Muestra el estado de los archivos
git pull origin [branch]      -> Trae todos los cambios que se hicieron en el branch seleccionado
git commit -m "comentario"    -> Hace un commit poniendole un comentario (si no se pushea no cambia nada en el repositorio)
git push origin [branch]      -> Manda todos los archivos commiteados al branch
git checkout [branch]         -> Cambia de branch al branch seleccionado
git checkout -b [branch]      -> Crea un nuevo branch y cambia a este branch
git branch                    -> Muestra todos los branches que hay y marca con un asterisco el que estés usando actualmente
git fetch                     -> Si cambian de máquina con esto actualizan todos los branches que hay en el repositorio
git merge [branch]            -> Trae todos los cambios que haya en el branch seleccionado y los mergea en el branch en el que estás parado
