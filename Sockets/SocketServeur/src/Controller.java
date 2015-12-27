import java.io.*;
import java.net.Socket;

/**
 * Created by paulnta on 27.12.15.
 */
public class Controller extends Thread{

    private Socket socket;
    private BufferedReader reader;
    private PrintWriter writer;

    public Controller(Socket socket){
        this.socket = socket;

        try {
            reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            writer = new PrintWriter(socket.getOutputStream());

        } catch (IOException e) {
            e.printStackTrace();
        }

        this.start();
    }

    public void run(){

        System.out.println("running");

        try {

            boolean stop = false;
            do {
                String line = reader.readLine();

                System.out.println("received: " + line);

                if(line == null || line.equals("STOP")){
                    System.out.println("Ok thanks");
                    stop = true;
                }

            }while(!stop);

            socket.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
