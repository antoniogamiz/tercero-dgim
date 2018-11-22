/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Ejercicio5;

/**
 *
 * @author antonio
 */
public class Transmitter extends Thread {
    @Override
    public void run () {
        User user;
        do {
            for( int i = 0; i < Server.usersConnected.size(); i++ ) {
                user = Server.usersConnected.get(i);
                if ( user.isLogin() )
                    Protocol.emitPacket( user.getOutPrinter(), Protocol.DATA, "data data data" );
            }
            try { Thread.sleep(1000); } catch (InterruptedException e) {};
        } while ( true );
    }
}
