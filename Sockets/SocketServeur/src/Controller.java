import java.awt.AWTException;
import java.awt.Robot;
import java.awt.event.InputEvent;
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
            String line;
            String splitedLine[];
            Robot mouseController = new Robot();
            do {
                line = reader.readLine();
                splitedLine = line.split(":");
                System.out.println("received: " + line);
                
                switch (splitedLine[0]) {
				case "release":
					System.out.println("MOUSE RELEASED");
					mouseController.mouseRelease(InputEvent.BUTTON1_MASK);
					break;
				case "press":
					System.out.println("MOUSE PRESSED");
					mouseController.mousePress(InputEvent.BUTTON1_MASK);
					break;
				case "move" : 
					System.out.println("MOUSE MOVED");
					int x = Integer.valueOf(splitedLine[1]);
					int y = Integer.valueOf(splitedLine[2]);
					mouseController.mouseMove(x, y);
					break;
				case "quit" :
					stop = true;
					System.out.println("QUIT");
					break;

				default:
					System.out.println("Wrong command format");
					break;
				}
                
            }while(!stop);

            socket.close();

        } catch (IOException | AWTException e) {
            e.printStackTrace();
        }
    }

}
