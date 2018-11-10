package Ejercicio3;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

//
// YodafyServidorIterativo
// (CC) jjramos, 2012
//
public class YodafyServidorIterativo {

	public static void main(String[] args) {
	
		// Puerto de escucha
		int port=8989;
		// array de bytes auxiliar para recibir o enviar datos.
		byte []buffer=new byte[256];
		// Número de bytes leídos
		int bytesLeidos=0;
		
                
                ServerSocket socketServidor;
		try {
                    socketServidor = new ServerSocket (port);
                    System.out.println("Servidor escuchando en " + Integer.toString(port));
                    Socket socketServicio = null;
                    do {
                        try{
                         socketServicio = socketServidor.accept();   
                        } catch (IOException e) {
                            System.out.println("Error: no se pudo aceptar la conexión solicitada.");
                        }

                        // Creamos un objeto de la clase ProcesadorYodafy, pasándole como 
			// argumento el nuevo socket, para que realice el procesamiento
			// Este esquema permite que se puedan usar hebras más fácilmente.
			ProcesadorYodafy procesador=new ProcesadorYodafy(socketServicio);
			procesador.start();
				
			} while (true);
			
		} catch (IOException e) {
			System.err.println("Error al escuchar en el puerto "+port);
		}

	}

}
