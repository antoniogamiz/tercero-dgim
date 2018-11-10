package Ejercicio4;

//
// YodafyServidorIterativo
// (CC) jjramos, 2012
//
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class YodafyClienteTCP {

	public static void main(String[] args) {
		
		byte []buferEnvio;
		byte []buferRecepcion=new byte[256];
		int bytesLeidos=0;
		
		// Nombre del host donde se ejecuta el servidor:
		String host="localhost";
		// Puerto en el que espera el servidor:
		int port=8989;
		
		// Socket para la conexión TCP
		DatagramSocket socketServicio=null;
                DatagramPacket receivePacket, sendPacket;
                InetAddress direccion;
		String respuesta;
		try {
                    do{
                        socketServicio = new DatagramSocket();
                        direccion = InetAddress.getByName( host );
                        
                        buferEnvio="Al monte del volcán debes ir sin demora".getBytes();
			
                        sendPacket = new DatagramPacket( buferEnvio, buferEnvio.length, direccion, port );
                        socketServicio.send( sendPacket );
                        
                        receivePacket = new DatagramPacket( buferRecepcion, buferRecepcion.length );
                        socketServicio.receive( receivePacket );
                        respuesta = new String( receivePacket.getData() );
                        System.out.println( "Respuesta recibida: " + respuesta );
			
                        socketServicio.close();
                    } while (true);
                // Excepciones:
		} catch (UnknownHostException e) {
			System.err.println("Error: Nombre de host no encontrado.");
		} catch (IOException e) {
			System.err.println("Error de entrada/salida al abrir el socket.");
		}
	}
}
