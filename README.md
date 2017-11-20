# isa-popcl
__isa-popcl__ is secured POP3 Client programmed in C++, which downloads mail from your inbox to specific folder or deletes it from server.
It supports unsecured connection, secured connection POP3s or STARTTLS connection.

### Install
*Installation is made using Makefile:*
```sh
$ make
```

### Run example
```sh
$ ./popcl pop3.seznam.cz -p 110 -T -n -o maildir -a auth_file.txt
```
### Additions & Limitations
- If bad port specified using nonsecure connection, client must be closed manually using __CTRL+C__.


### Help
- Help prints using atributes `-h`, `--help` or after running program without arguments. 
```
Program usage:
popcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]] [-d] [-n] -a <auth_file> -o <out_dir>
     -p   port            - sets remote TCP port
     -T   pop3s           - sets pop3s secured connection
     -S   STSL            - connects with STLS (RFC 2595)
     -c   certfile        - path to certificate file
     -C   certfolder      - path to folder with certificates
     -d   delete          - deletes all messages from server
     -n   new messages    - works only with new messages
     -a   authfile        - path to authentication file
     -o   outdir          - folder where messages will be stored
```

### Authentization File

Authentization file contains username and password in strictly given order, natively in UNIX/Linux.

> username = my_name
> password = my_password

### Project Files
 - `src/args.cpp`
 - `src/args.h`
 - `src/config.h`
 - `src/connection.cpp`
 - `src/connection.h`
 - `src/error.h`
 - `src/fileman.cpp`
 - `src/pop3man.cpp`
 - `src/pop3man.h`
 - `Makefile`
 - `Readme.md`
 - `popcl.cpp`