#include "renderizador.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Tablero* Renderizador::tablero = nullptr;
Juego* Renderizador::juego = nullptr;
EstadoApp estadoActual = EstadoApp::MENU_PRINCIPAL;
ModoJuego modoSeleccionado = ModoJuego::MODO_5x6;
bool tipoVsMaquina = false;
DificultadBot dificultadSeleccionada = DificultadBot::NOOB;
std::map<std::string, GLuint> Renderizador::texturasPiezas;

std::string Renderizador::mensajeEstado = "";

GLuint Renderizador::texturaFondo = 0;
EstiloVisual Renderizador::estiloActual = EstiloVisual::NORMAL;

float Renderizador::alphaTablero = 0.0f;






GLuint cargarTextura(const std::string& ruta) {
    int ancho, alto, canales;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* datos = stbi_load(ruta.c_str(), &ancho, &alto, &canales, 4); // fuerza RGBA
    if (!datos) {

        return 0;
    }

    GLuint texturaID;
    glGenTextures(1, &texturaID);
    glBindTexture(GL_TEXTURE_2D, texturaID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ancho, alto, 0, GL_RGBA, GL_UNSIGNED_BYTE, datos);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(datos);
    return texturaID;
}


void Renderizador::establecerJuego(Juego* j) {
    juego = j;
}

void Renderizador::inicializar(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 700);
    //
    glutCreateWindow("Ajedrez Personalizado");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    gluOrtho2D(-1, 6, -1, 7);

    glClearColor(1, 1, 1, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cargarTexturasPiezas();
}

void Renderizador::establecerTablero(Tablero* t) {
    tablero = t;
}

void Renderizador::dibujar() {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (estadoActual) {
    case EstadoApp::MENU_PRINCIPAL:
        mostrarMenu();
        break;
        //mio
    case EstadoApp::SELECCION_ESTILO:
        mostrarSeleccionEstilo();
        break;
        //
    case EstadoApp::SELECCION_MODO:
        mostrarSeleccionModo();
        break;
    case EstadoApp::SELECCION_TIPO_JUEGO:
        mostrarSeleccionTipoJuego();
        break;
    case EstadoApp::SELECCION_DIFICULTAD:
        mostrarSeleccionDificultad();
        break;
    case EstadoApp::FIN_PARTIDA:
        mostrarPantallaFinPartida();
        break;
    case EstadoApp::CORONACION_PEON:
        mostrarOpcionesCoronacion();
        break;
    case EstadoApp::SOLICITUD_TABLAS:
        mostrarSolicitudTablas(Renderizador::mensajeEstado);
        break;
    case EstadoApp::JUEGO:


        if (tablero) {

            //MIO PARA FONDO
            if (texturaFondo != 0) {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texturaFondo);
                glColor3f(1.0f, 1.0f, 1.0f);

                // Dibujar el fondo en toda el área visible (-1 a 6 en X, -1 a 7 en Y)
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(-1, -1);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(6, -1);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(6, 7);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(-1, 7);
                glEnd();

                glDisable(GL_TEXTURE_2D);
            }

            ///

            for (int fila = 0; fila < 6; ++fila)
                for (int col = 0; col < 5; ++col) {
                    dibujarCasilla(fila, col);
                    Pieza* pieza = tablero->obtenerPieza(Posicion(fila, col));
                    if (pieza)
                        dibujarPieza(pieza, fila, col);
                }
        }
        break;

    }



    glutSwapBuffers();
}



void Renderizador::dibujarCasilla(int fila, int col) {

    if ((fila + col) % 2 == 0)
        glColor4f(0.8f, 0.8f, 0.8f, alphaTablero);
    else
        glColor4f(0.3f, 0.3f, 0.3f, alphaTablero);


    glBegin(GL_POLYGON);
    glVertex2f(col, fila);
    glVertex2f(col + 1, fila);
    glVertex2f(col + 1, fila + 1);
    glVertex2f(col, fila + 1);
    glEnd();
}

void Renderizador::dibujarPieza(Pieza* pieza, int fila, int col) {
    if (!pieza) return;

    std::string clave = "";
    clave += pieza->getSimbolo();  // Ej: 'P', 'R', 'A', etc.
    clave += "_";
    clave += (pieza->getColor() == Color::BLANCO ? "B" : "N");

    GLuint textura = texturasPiezas[clave];
    if (textura == 0) return;

    float x = col;
    float y = fila;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textura);


    if (alphaTablero < 0.7f)
    {
        glColor4f(1.0f, 1.0f, 1.0f, alphaTablero);  // Usar alpha actual
    }
    else {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);  // Usar alpha actual
    }

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(x + 1, y);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(x + 1, y + 1);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + 1);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}


void Renderizador::mostrarMenu() {
    glColor3f(0, 0, 0);
    glRasterPos2f(1.5f, 4.5f);
    const char* titulo = "AJEDREZ KRAMNIK";
    for (const char* c = titulo; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    glColor3f(0.2f, 0.6f, 0.9f); // Color del botón
    glBegin(GL_POLYGON);
    glVertex2f(2, 2);
    glVertex2f(3, 2);
    glVertex2f(3, 2.5);
    glVertex2f(2, 2.5);
    glEnd();

    glColor3f(1, 1, 1);
    glRasterPos2f(2.25f, 2.2f);
    const char* play = "PLAY";
    for (const char* c = play; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void Renderizador::cambiarAEstadoJuego() {
    estadoActual = EstadoApp::JUEGO;
    glutPostRedisplay(); // Forzar redibujo
}

EstadoApp Renderizador::obtenerEstadoActual() {
    return estadoActual;
}

void Renderizador::manejarMouse(int boton, int estado, int x, int y) {
    if (boton != GLUT_LEFT_BUTTON || estado != GLUT_DOWN) return;

    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);

    //CAMBIADO NUEVAS COORDENADAS
    float xOpenGL = x * 7.0f / ancho - 1.0f;
    float yOpenGL = (alto - y) * 8.0f / alto - 1.0f;


    if (estadoActual == EstadoApp::MENU_PRINCIPAL) {
        // Botón PLAY
        if (xOpenGL >= 2 && xOpenGL <= 3 && yOpenGL >= 2 && yOpenGL <= 2.5) {
            estadoActual = EstadoApp::SELECCION_ESTILO;
            glutPostRedisplay();
        }
    }

    else if (estadoActual == EstadoApp::SELECCION_MODO) {
        // Botón Modo 5x6
        if (xOpenGL >= 1 && xOpenGL <= 4 && yOpenGL >= 3 && yOpenGL <= 3.5) {
            modoSeleccionado = ModoJuego::MODO_5x6;
            estadoActual = EstadoApp::SELECCION_TIPO_JUEGO;
            glutPostRedisplay();
        }
        // Botón Modo Petty
        else if (xOpenGL >= 1 && xOpenGL <= 4 && yOpenGL >= 2 && yOpenGL <= 2.5) {
            modoSeleccionado = ModoJuego::MODO_PETTY;
            estadoActual = EstadoApp::SELECCION_TIPO_JUEGO;
            glutPostRedisplay();
        }
    }

    else if (estadoActual == EstadoApp::SELECCION_TIPO_JUEGO) {
        if (xOpenGL >= 1 && xOpenGL <= 4) {
            if (yOpenGL >= 3 && yOpenGL <= 3.5) {
                tipoVsMaquina = false;
                estadoActual = EstadoApp::JUEGO;
                iniciarJuegoSegunModo();
                glutPostRedisplay();
            }
            else if (yOpenGL >= 2 && yOpenGL <= 2.5) {
                tipoVsMaquina = true;
                estadoActual = EstadoApp::SELECCION_DIFICULTAD;
                glutPostRedisplay();
            }
        }
    }
    else if (estadoActual == EstadoApp::SELECCION_DIFICULTAD) {
        if (xOpenGL >= 1 && xOpenGL <= 4) {
            if (yOpenGL >= 3 && yOpenGL <= 3.5) {
                dificultadSeleccionada = DificultadBot::NOOB;
            }
            else if (yOpenGL >= 2 && yOpenGL <= 2.5) {
                dificultadSeleccionada = DificultadBot::MID;
            }
            estadoActual = EstadoApp::JUEGO;
            iniciarJuegoSegunModo();
            glutPostRedisplay();
        }
    }
    else if (estadoActual == EstadoApp::SELECCION_ESTILO) {
        if (xOpenGL >= 1.0f && xOpenGL <= 4.0f) {
            if (yOpenGL >= 3.0f && yOpenGL <= 3.5f) {
                estiloActual = EstiloVisual::NORMAL;
                cargarTexturasPiezas();              // Cargar el estilo elegido
                estadoActual = EstadoApp::SELECCION_MODO;  // Ir al menú principal
                glutPostRedisplay();
            }
            else if (yOpenGL >= 2.0f && yOpenGL <= 2.5f) {
                estiloActual = EstiloVisual::BARAJA;
                cargarTexturasPiezas();              // Cargar el estilo elegido
                estadoActual = EstadoApp::SELECCION_MODO;  // Ir al menú principal
                glutPostRedisplay();
            }


        }
    }


}


void Renderizador::mostrarSeleccionModo() {
    glColor3f(0, 0, 0);
    glRasterPos2f(1.0f, 5.0f);
    const char* texto = "Selecciona el modo de juego:";
    for (const char* c = texto; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón Modo 5x6
    glColor3f(0.4f, 0.7f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(1, 3);
    glVertex2f(4, 3);
    glVertex2f(4, 3.5);
    glVertex2f(1, 3.5);
    glEnd();

    glColor3f(1, 1, 1);
    glRasterPos2f(1.5f, 3.2f);
    const char* texto5x6 = "Modo 5x6";
    for (const char* c = texto5x6; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón Modo Petty
    glColor3f(0.8f, 0.3f, 0.3f);
    glBegin(GL_POLYGON);
    glVertex2f(1, 2);
    glVertex2f(4, 2);
    glVertex2f(4, 2.5);
    glVertex2f(1, 2.5);
    glEnd();

    glColor3f(1, 1, 1);
    glRasterPos2f(1.5f, 2.2f);
    const char* textoPetty = "Modo Petty";
    for (const char* c = textoPetty; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void Renderizador::mostrarSeleccionTipoJuego() {
    glColor3f(0, 0, 0);
    glRasterPos2f(1.0f, 4.5f);
    const char* texto = "Selecciona tipo de juego:";
    for (const char* c = texto; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón Persona vs Persona
    glColor3f(0.2f, 0.6f, 0.9f);
    glBegin(GL_POLYGON);
    glVertex2f(1, 3);
    glVertex2f(4, 3);
    glVertex2f(4, 3.5);
    glVertex2f(1, 3.5);
    glEnd();

    glColor3f(1, 1, 1);
    glRasterPos2f(1.4f, 3.2f);
    const char* pvp = "Persona vs Persona";
    for (const char* c = pvp; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón Persona vs Máquina
    glColor3f(0.2f, 0.6f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(1, 2);
    glVertex2f(4, 2);
    glVertex2f(4, 2.5);
    glVertex2f(1, 2.5);
    glEnd();

    glColor3f(1, 1, 1);
    glRasterPos2f(1.5f, 2.2f);
    const char* pvc = "Persona vs Maquina";
    for (const char* c = pvc; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void Renderizador::mostrarSeleccionDificultad() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(1.5f, 4.5f);
    const char* titulo = "Selecciona la dificultad del bot:";
    for (const char* c = titulo; *c != '\0'; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón NOOB
    glColor3f(0.2f, 0.6f, 0.2f); // Verde
    glBegin(GL_QUADS);
    glVertex2f(1.0f, 3.0f);
    glVertex2f(4.0f, 3.0f);
    glVertex2f(4.0f, 3.5f);
    glVertex2f(1.0f, 3.5f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(2.2f, 3.2f);
    const char* textoNoob = "NOOB";
    for (const char* c = textoNoob; *c != '\0'; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón MID
    glColor3f(0.6f, 0.2f, 0.2f); // Rojo oscuro
    glBegin(GL_QUADS);
    glVertex2f(1.0f, 2.0f);
    glVertex2f(4.0f, 2.0f);
    glVertex2f(4.0f, 2.5f);
    glVertex2f(1.0f, 2.5f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(2.3f, 2.2f);
    const char* textoMid = "MID";
    for (const char* c = textoMid; *c != '\0'; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void Renderizador::iniciarJuegoSegunModo() {
    Juego* juego = Renderizador::juego;
    if (!juego) return;

    if (modoSeleccionado == ModoJuego::MODO_5x6)
        juego->iniciar5x6();
    else
        juego->iniciarPetty();

    // Si es contra máquina y el turno actual es del bot (por ejemplo, siempre negras)
    if (tipoVsMaquina && juego->obtenerTurnoActual() == Color::NEGRO) {
        if (dificultadSeleccionada == DificultadBot::NOOB)
            juego->jugarTurnoBotNoob();
        else
            juego->jugarTurnoBotMid();
    }
}

void Renderizador::mostrarPantallaFinPartida() {
    // Fondo blanco
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Texto negro
    glColor3f(0.0f, 0.0f, 0.0f);

    // Mensaje de fin de partida
    glRasterPos2f(1.6f, 4.5f);
    std::string mensaje = "FIN DE LA PARTIDA";
    for (char c : mensaje)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    // Mensaje de estado (como "Tablas reclamadas", etc.)
    glRasterPos2f(1.2f, 2.2f);
    for (char c : mensajeEstado)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}


void Renderizador::mostrarOpcionesCoronacion() {
    // Fondo blanco
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Texto negro
    glColor3f(0.0f, 0.0f, 0.0f);

    // Primer mensaje
    glRasterPos2f(1.2f, 3.2f);
    std::string mensaje = "¡Peon ha llegado al final!";
    for (char c : mensaje)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    // Segundo mensaje
    glRasterPos2f(0.5f, 2.6f);
    std::string opciones = "Elige: R (Reina), T (Torre), A (Alfil), C (Caballo)";
    for (char c : opciones)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}


void Renderizador::mostrarSolicitudTablas(const std::string& mensajeEstado) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Fondo blanco
    glClear(GL_COLOR_BUFFER_BIT);

    // Establecer sistema de coordenadas ortográficas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 6, -1, 7);  // Mismo sistema que mostrarMenu

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.0f, 0.0f, 0.0f);  // Texto negro

    // Título
    glRasterPos2f(1.2f, 5.0f);
    std::string linea1 = "¿Deseas reclamar tablas?";
    for (char c : linea1)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    // Mensaje del estado
    glRasterPos2f(0.8f, 4.6f);
    for (char c : mensajeEstado)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    // Botones: SÍ y NO

    float x_si1 = 1.2f, y_si1 = 3.0f;
    float x_si2 = 2.2f, y_si2 = 3.6f;

    float x_no1 = 2.8f, y_no1 = 3.0f;
    float x_no2 = 3.8f, y_no2 = 3.6f;

    // Dibujar botones
    glColor3f(0.8f, 0.8f, 0.8f); // gris claro

    glBegin(GL_QUADS); // Botón Sí
    glVertex2f(x_si1, y_si1);
    glVertex2f(x_si2, y_si1);
    glVertex2f(x_si2, y_si2);
    glVertex2f(x_si1, y_si2);
    glEnd();

    glBegin(GL_QUADS); // Botón No
    glVertex2f(x_no1, y_no1);
    glVertex2f(x_no2, y_no1);
    glVertex2f(x_no2, y_no2);
    glVertex2f(x_no1, y_no2);
    glEnd();

    // Texto dentro de botones
    glColor3f(0.0f, 0.0f, 0.0f);

    glRasterPos2f(1.55f, 3.25f);
    for (char c : std::string("Si"))
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    glRasterPos2f(3.15f, 3.25f);
    for (char c : std::string("No"))
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}


std::string Renderizador::obtenerNombreArchivo(const std::string& clave) {
    if (estiloActual == EstiloVisual::NORMAL) {
        if (clave == "P_B") return "P_B.png";
        if (clave == "p_N") return "p_N.png";
        if (clave == "T_B") return "T_B.png";
        if (clave == "t_N") return "t_N.png";
        if (clave == "C_B") return "C_B.png";
        if (clave == "c_N") return "c_N.png";
        if (clave == "A_B") return "A_B.png";
        if (clave == "a_N") return "a_N.png";
        if (clave == "Q_B") return "Q_B.png";
        if (clave == "q_N") return "q_N.png";
        if (clave == "R_B") return "R_B.png";
        if (clave == "r_N") return "r_N.png";
    }
    else if (estiloActual == EstiloVisual::BARAJA) {


        texturaFondo = cargarTextura("fondos/fondos_baraja.png"); // Ruta al fondo de la baraja


        if (clave == "P_B") return "PEON_DE_COPAS.png";   // ← PEÓN blanco en estilo Star Wars
        if (clave == "p_N") return "PEON_DE_OROS.png";    // ← PEÓN negro
        if (clave == "T_B") return "TORRE_DE_COPAS.png";            // ← TORRE blanca
        if (clave == "t_N") return "TORRE_DE_OROS.png";      // ← TORRE negra
        if (clave == "C_B") return "CABALLO_DE_COPAS.png";        // ← CABALLO blanco
        if (clave == "c_N") return "CABALLO_DE_OROS.png";            // ← CABALLO negro
        if (clave == "A_B") return "ALFIL_DE_COPAS.png";             // ← ALFIL blanco
        if (clave == "a_N") return "ALFIL_DE_OROS.png";        // ← ALFIL negro
        if (clave == "Q_B") return "REINA_DE_COPAS.png";              // ← REINA blanca
        if (clave == "q_N") return "REINA_DE_OROS.png";         // ← REINA negra
        if (clave == "R_B") return "REY_DE_COPAS.png";             // ← REY blanco
        if (clave == "r_N") return "REY_DE_OROS.png";            // ← REY negro
    }

    return ""; // En caso de error
}

void Renderizador::cargarTexturasPiezas() {
    texturasPiezas.clear();
    std::string carpeta = (estiloActual == EstiloVisual::NORMAL) ? "imagenes/" : "baraja/";

    std::vector<std::string> claves = {
        "P_B", "p_N", "T_B", "t_N", "C_B", "c_N",
        "A_B", "a_N", "Q_B", "q_N", "R_B", "r_N"
    };

    for (const auto& clave : claves) {
        std::string archivo = obtenerNombreArchivo(clave);
        if (!archivo.empty()) {
            texturasPiezas[clave] = cargarTextura(carpeta + archivo);
        }
    }


    //FONDO PARA BARAJA DE LA ETSIDI
    if (estiloActual == EstiloVisual::BARAJA) {
        texturaFondo = cargarTextura("fondos/fondos_baraja.png");
    }
    else if (estiloActual == EstiloVisual::NORMAL)//FONDO PARA MODO NORMAL
    {
        texturaFondo = cargarTextura("fondos/fondo_car.png");
        //texturaFondo = cargarTextura("fondos/fondo_san.png");

    }
    else {
        texturaFondo = 0; // No usar fondo en otros estilos
    }

}


//METODO PARA QUE EL TABLERO Y LAS PIEZAS APAREZCAN GRADUALMENTE
void Renderizador::actualizarAlpha(int valor) {
    if (alphaTablero < 0.7f) {
        alphaTablero += 0.05f;
        if (alphaTablero > 0.7f) alphaTablero = 0.7f;
        glutPostRedisplay();
        glutTimerFunc(30, actualizarAlpha, 0);  // Llama otra vez en 30ms
    }
}


void Renderizador::mostrarSeleccionEstilo() {
    glColor3f(0, 0, 0);
    glRasterPos2f(1.2f, 5.0f);
    const char* titulo = "Selecciona el estilo de piezas:";
    for (const char* c = titulo; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón Normal
    glColor3f(0.2f, 0.6f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(1.0f, 3.0f);
    glVertex2f(4.0f, 3.0f);
    glVertex2f(4.0f, 3.5f);
    glVertex2f(1.0f, 3.5f);
    glEnd();

    glColor3f(1, 1, 1);
    glRasterPos2f(1.7f, 3.2f);
    const char* textoNormal = "Estilo Normal";
    for (const char* c = textoNormal; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón BARAJA ESPAÑOLA
    glColor3f(0.7f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(1.0f, 2.0f);
    glVertex2f(4.0f, 2.0f);
    glVertex2f(4.0f, 2.5f);
    glVertex2f(1.0f, 2.5f);
    glEnd();

    glColor3f(1, 1, 1);
    glRasterPos2f(1.6f, 2.2f);
    const char* textoSW = "Estilo Baraja Española";
    for (const char* c = textoSW; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}


void Renderizador::setEstadoActual(EstadoApp nuevoEstado) {
    estadoActual = nuevoEstado;
}

EstadoApp Renderizador::getEstadoActual() {
    return estadoActual;
}
//TEMPORIZADOR PARA MODO PERSONA VS PERSONA, YA NO APARECE EN MQUINA VS PERSONA
void Renderizador::dibujarTemporizador() {
    if (!juego) return;

    int tB = juego->obtenerTiempoBlanco();
    int tN = juego->obtenerTiempoNegro();

    std::string textoB = "Blancas: " + std::to_string(tB) + "s";
    std::string textoN = "Negras: " + std::to_string(tN) + "s";

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(0.5f, 6.7f);
    for (char c : textoB) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    glRasterPos2f(3.0f, 6.7f);
    for (char c : textoN) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}
// CALCULO DE TIEMPO PARA NEGRAS Y BLAMCAS


int Juego::obtenerTiempoBlanco() const {

    if ((turnoActual == Color::BLANCO) && (estadoActual == EstadoApp::JUEGO)) {
        auto ahora = std::chrono::steady_clock::now();
        int pasado = std::chrono::duration_cast<std::chrono::seconds>(ahora - inicioTurno).count();
        return std::max(0, tiempoBlanco - pasado);
    }
    return tiempoBlanco;
}

int Juego::obtenerTiempoNegro() const {
    if ((turnoActual == Color::NEGRO) && (estadoActual == EstadoApp::JUEGO)) {
        auto ahora = std::chrono::steady_clock::now();
        int pasado = std::chrono::duration_cast<std::chrono::seconds>(ahora - inicioTurno).count();
        return std::max(0, tiempoNegro - pasado);
    }
    return tiempoNegro;
}