/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Ejercicio5;

import java.io.BufferedReader;
import java.io.PrintWriter;

/**
 *
 * @author antonio
 * Creamos esta clase para "abstraer" cómo el protocolo emite los paquetes o qué códigos usa
 * para cada paquete.
 */
public class Protocol {
    static int DISCONNECT = 1000;
    static int LOGIN = 1001;
    static int REGISTRATION = 1002;
    static int CONNECTIONTIME = 1003;
    static int OK = 200;
    static int LOGINERROR = 301;
    static int USERTAKEN = 302;

    
    static int DATA = 2000;
    
    static String SEPARATOR = "//";
    
    public static void emitPacket( PrintWriter outPrinter, int code, String message  ) {
        String packetMessage = Integer.toString(code) + Protocol.SEPARATOR + message;
        outPrinter.println( packetMessage );
        outPrinter.flush();
    }
    
    public static String[] receivePacket( BufferedReader inReader ) {
        String[] message = null;
        try { 
            message = inReader.readLine().split( SEPARATOR ); 
        } catch ( Exception e ) {};
        return message;
    }

}
