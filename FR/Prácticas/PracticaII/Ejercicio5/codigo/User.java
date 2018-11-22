/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Ejercicio5;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;

/**
 *
 * @author antonio
 */
public class User extends Thread {
    
    Socket serviceSocket;
    PrintWriter outPrinter;
    BufferedReader inReader;
    String username = "";
    String password = "";
    int connectionTime = 0;
    
    boolean login = false;
    
    User( Socket s ) {
        this.serviceSocket = s;

        try {
            // Obtiene los flujos de escritura/lectura
            outPrinter = new PrintWriter( serviceSocket.getOutputStream(), true );
            inReader = new BufferedReader( new InputStreamReader( serviceSocket.getInputStream() ) );
        } catch ( IOException e ){
            System.err.println( "Error getting input/output flows." );
        }        
    }

    public PrintWriter getOutPrinter() {
        return outPrinter;
    }
    
    public void identify() {
        boolean identificationSuccessful = false;
        do {
            String[] messageReceived = Protocol.receivePacket( inReader );
            int code = Integer.parseInt( messageReceived[0] );

            String[] credentials = messageReceived[1].split( " " );

            // El usuario debe identificarse antes de hacer cualquier otra cosa.
            if ( code != Protocol.LOGIN && code != Protocol.REGISTRATION ) {
                Protocol.emitPacket( outPrinter, Protocol.LOGINERROR, "You must be identified before doing any other operation." );
            } // Manejar el login de un usuario
            else if ( code == Protocol.LOGIN ) {
                if ( Server.checkUsernamePassword( credentials[0], credentials[1]) ) {
                    Protocol.emitPacket( outPrinter, Protocol.OK, "Login successful" );
                    identificationSuccessful = true;
                    username = credentials[0]; password = credentials[1];
                } else {
                    Protocol.emitPacket( outPrinter, Protocol.LOGINERROR, "Username or password incorrect." );
                }
            } // Registrar nuevo usuario
            else if ( code == Protocol.REGISTRATION ) {
                if ( Server.registerNewUser( credentials[0], credentials[1] ) ) {
                    Protocol.emitPacket( outPrinter, Protocol.OK, "Registration successful." );
                    identificationSuccessful = true;
                    username = credentials[0]; password = credentials[1];
                } else {
                    Protocol.emitPacket( outPrinter, Protocol.USERTAKEN, "Username already exists." );
                }
            }
        } while ( !identificationSuccessful );
        
        login = true;
        
    }

    public boolean isLogin() {
        return login;
    }
    
    @Override
    public void run() {        
        identify();
        String messageReceived[];
        int code;
        boolean connected = true;
        
        //Guardamos cada cuanto tiempo quiere que le preguntemos si 
        //desea seguir conectado
        messageReceived = Protocol.receivePacket( inReader );
        code = Integer.parseInt( messageReceived[0] );
        if( code == Protocol.CONNECTIONTIME ) {
            connectionTime = Integer.parseInt( messageReceived[1] );
        }

        //Dormimos durante el tiempo fijado, mandamos paquete para 
        //ver si seguimos conectados o no.
        do{
            try { Thread.sleep( connectionTime ); } catch( Exception e ) {};
            Protocol.emitPacket(outPrinter, Protocol.STAYCONNECTED, "");
            messageReceived = Protocol.receivePacket( inReader );
            code = Integer.parseInt( messageReceived[0] );
            if( code == Protocol.STAYCONNECTEDANSWER ){
                if( "0".equals(messageReceived[1]) )
                    connected = false;
            }
        } while ( connected );

        Protocol.emitPacket(outPrinter, Protocol.DISCONNECT, "");
        System.out.println( "Closing connection..." );
        try { serviceSocket.close(); } catch ( Exception e ) {};
        System.out.println( "Connection closed. " + username + " disconnected.");

    }
}
