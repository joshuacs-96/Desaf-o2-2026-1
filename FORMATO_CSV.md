# Formato de persistencia

Los identificadores son `int` positivos y únicos. Los archivos guardan IDs, nunca direcciones de memoria. Al cargar, `RedSocial` resuelve cada ID a su objeto y establece los punteros requeridos por el diagrama de clases.

## Decisiones de implementación

Se usa `std::string` exclusivamente para almacenar texto. Las búsquedas y ordenamientos propios de la aplicación se implementan con ciclos sobre las estructuras del proyecto; no se usan contenedores ni algoritmos de búsqueda u ordenamiento de la STL. En memoria, los mensajes y solicitudes guardan punteros a sus usuarios participantes. En disco, esas relaciones se representan por IDs.

Los campos de fecha usan `YYYY-MM-DD HH:MM:SS`. Los textos no pueden contener salto de línea. Para incluir una coma, el campo se encierra entre comillas dobles; una comilla interna se escribe como dos comillas dobles. Ejemplo: `"Hola, ""amiga"""`.

- `usuarios.csv`: `id,nombreUsuario,fechaNacimiento,correo,contrasena,fotoPerfil,ciudad,pais,fechaInscripcion`
- `amistades.csv`: `idUsuario1,idUsuario2`
- `solicitudes.csv`: `idRemitente,idDestinatario,fechaHora`. Solo se persisten solicitudes pendientes; una aceptada se vuelve amistad y una rechazada se elimina.
- `albumes.csv`: `idAlbum,idPropietario,nombre,rutaAlmacenamiento,privacidad`, donde privacidad es `publico`, `amigos` o `privado`.
- `fotos.csv`: `idFoto,idAlbum,fechaCaptura,tamanoBytes,ruta,conteoLikes`. El conteo es derivable desde `likes.csv`; se conserva para facilitar la carga y debe coincidir con ese archivo.
- `likes.csv`: `idUsuario,idFoto`.
- `mensajes.csv`: `idEmisor,idReceptor,texto,fechaEnvio,fechaRecepcion,leido`, donde `leido` es `0` o `1`.
