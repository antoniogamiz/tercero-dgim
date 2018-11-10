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
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
/**
 *
 * @author antonio
 */
public class Server {
    
    static ArrayList<User> usersConnected = new ArrayList<User>();
    static ArrayList< String > usersRegistered = new ArrayList< String >();
    
    public static void main( String[] args ) {
        
        System.out.println( "Initialising ChatterServer..." );
        
        int port = 30000;
        ServerSocket serverSocket;
        
        try {
            
            serverSocket = new ServerSocket( port );
            System.out.println( "Server listening on port " + port );
            
            Socket serviceSocket = null;
            Transmitter t = new Transmitter();
            
            t.start();
            
            do{
                try {
                    serviceSocket = serverSocket.accept();
                    
                    System.out.println( "New user connection..." );
                    usersConnected.add( new User( serviceSocket ) );
                    usersConnected.get( usersConnected.size() - 1 ).start();
                    
                } catch ( IOException e ) {
                    System.out.println("Error: request could not be accepted.");
                }
            } while ( true );
            
            
        } catch ( IOException e ) {
            System.err.println( "Error listening port " + port );
        }
        
    }
    
    public static boolean registerNewUser( String username, String password ) {
        if ( checkUsernamePassword( username, password ) ) return false;
        
        usersRegistered.add( username + " " + password );
        return true;
    }
    
    public static boolean checkUsernamePassword( String username, String password ) {
        String[] user;
        for( int i = 0; i < usersRegistered.size(); i++ ) {
            user = usersRegistered.get(i).split( " " );
            if ( user[0].equals(username) )
                return true;
        }
        return false;
    }
}
