import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Created by paulnta on 27.12.15.
 */

public class Server implements Runnable{


    private ServerSocket serverSocket;
    private static final int PORT = 5003;

    public Server(){
        try {
            serverSocket = new ServerSocket(PORT);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String ... args){
        new Server().run();
        System.out.println("finished");
    }

    public void run(){
        System.out.println("waiting for connection");

            try {
                Socket socket = serverSocket.accept();
                System.out.println("new client");
                new Controller(socket).join();


            } catch (IOException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
    }

}
