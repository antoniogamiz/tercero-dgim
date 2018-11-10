package Ejercicio2;

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
import java.net.Socket;
import java.net.UnknownHostException;

public class YodafyClienteTCP {

	public static void main(String[] args) {
		
		String charEnvio;
		byte []buferRecepcion=new byte[256];
                String respuesta;
		// Nombre del host donde se ejecuta el servidor:
		String host="localhost";
		// Puerto en el que espera el servidor:
		int port=8989;
		
		// Socket para la conexión TCP
		Socket socketServicio=null;
		
		try {
                    socketServicio = new Socket (host, port);


                    InputStream inputStream = socketServicio.getInputStream();
                    OutputStream outputStream = socketServicio.getOutputStream();			
                    
                    PrintWriter outPrinter = new PrintWriter( outputStream, true );
                    BufferedReader inReader = new BufferedReader( new InputStreamReader( inputStream ) );
                    // Si queremos enviar una cadena de caracteres por u OutputStream, hay que pasarla primero
                    // a un array de bytes:
                    charEnvio="Al monte del volcán debes ir sin demora";

                    outPrinter.println(charEnvio);
                    respuesta = inReader.readLine();

                    // MOstremos la cadena de caracteres recibidos:
                    System.out.println("Recibido: ");
                    System.out.println(respuesta);

                    socketServicio.close();

                    // Excepciones:
		} catch (UnknownHostException e) {
			System.err.println("Error: Nombre de host no encontrado.");
		} catch (IOException e) {
			System.err.println("Error de entrada/salida al abrir el socket.");
		}
	}
}
