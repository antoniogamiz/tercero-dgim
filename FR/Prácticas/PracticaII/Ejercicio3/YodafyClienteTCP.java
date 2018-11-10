package Ejercicio3;

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

                    charEnvio="Al monte del volcán debes ir sin demora";

                    do {
                        System.out.println( "Cliente: pendiente de escribir respuesta." );
                        outPrinter.println(charEnvio);
                        outPrinter.flush();
                        System.out.println( "Cliente: respuesta escrita." );
                        System.out.println( "Cliente: Pendiente de leer respuesta." );
                        respuesta = inReader.readLine();
                        System.out.println( "Cliente: respuesta leida" );

                        // MOstremos la cadena de caracteres recibidos:
                        System.out.println("Recibido: ");
                        System.out.println(respuesta);                    
                    } while (true);
                    
                    //socketServicio.close();

                    // Excepciones:
		} catch (UnknownHostException e) {
			System.err.println("Error: Nombre de host no encontrado.");
		} catch (IOException e) {
			System.err.println("Error de entrada/salida al abrir el socket.");
		}
	}
}
