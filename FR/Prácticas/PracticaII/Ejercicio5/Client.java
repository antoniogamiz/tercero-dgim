/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Ejercicio5;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

/**
 *
 * @author antonio
 */
public class Client {
    
    static Socket serviceSocket;
    static PrintWriter outPrinter;
    static BufferedReader inReader;
    static String username;
    static String password;
    
    public static void main ( String[] args ) {
        
        System.out.println( "Initialising ChatterClient..." );

        int port = 30000;
        String host = "localhost";
        
        try {
            serviceSocket = new Socket( host, port );
         
            outPrinter = new PrintWriter( serviceSocket.getOutputStream(), true );
            inReader = new BufferedReader( new InputStreamReader( serviceSocket.getInputStream() ) );
            
            identify();
            boolean connected = true;
            String[] messageReceived;
            int code;
            Scanner S = new Scanner( System.in );
            System.out.println( "¿How long do you want to receive data (in ms) ?" );
            String time = S.nextLine();
            Protocol.emitPacket( outPrinter, Protocol.CONNECTIONTIME, time );
            messageReceived = Protocol.receivePacket( inReader );
            
            do{
                messageReceived = Protocol.receivePacket( inReader );
                code = Integer.parseInt( messageReceived[0] );
                
                if ( code == Protocol.DATA ) {
                    System.out.println( "Data Packet received: " + messageReceived[1] );
                } else if ( code == Protocol.DISCONNECT ) {
                    connected = false;
                    System.out.println( "Connection time expired. Disconnecting..." );
                }
                
            } while( connected );
            
           
        } catch (UnknownHostException e) {
                System.err.println("Error: host name not found.");
        } catch (IOException e) {
                System.err.println("Input/Output opening the socket.");
        }        
    }
    
    public static void identify() {
        Scanner S = new Scanner( System.in );
        boolean successfulLogin = false;
        
        String option;
        do {
            System.out.println( "Enter 0 if you want to login, 1 if you want to register: " );
            option = S.nextLine();    
        } while ( !option.equals("0") && !option.equals("1"));
        
        int codeID = option.equals("0") ? Protocol.LOGIN : Protocol.REGISTRATION;
        do {
            System.out.println( "Introduce your username: " );
            username = S.nextLine();
            System.out.println( "Introduce your password: " );
            password = S.nextLine();

            Protocol.emitPacket( outPrinter, codeID, username + " " + password );

            String[] messageReceived = Protocol.receivePacket( inReader );

            int code = Integer.parseInt( messageReceived[0] );
            if( code == Protocol.LOGINERROR || code == Protocol.USERTAKEN) {
                System.out.println( "Error: " + messageReceived[1] );
            } else {
                if ( code == Protocol.OK ) {
                    successfulLogin = true;
                    System.out.println( messageReceived[1] );
                }
            }

        } while ( !successfulLogin );

        System.out.println( "Successful identification..." );
    }
}
