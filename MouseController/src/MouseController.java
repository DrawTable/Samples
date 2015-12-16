import java.awt.*;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.geom.Point2D;

public class MouseController {

    Robot robot = new Robot();
    Point2D cercleTool = new Point(1410, 284);

    public static void main(String[] args) throws AWTException {
        new MouseController();
    }

    public MouseController() throws AWTException {
        // auto delay after each events
        robot.setAutoDelay(40);
        robot.setAutoWaitForIdle(true);
        robot.delay(5000);
        this.drawSomething();
        this.drawCercle(new Point(846,467), new Point(992,603));
        this.drawCercle(new Point(100,100), new Point(300,300));
        this.drawCercle(new Point(500,500), new Point(900,200));
    }


    public void drawCercle(Point start, Point end){
        selectTool(cercleTool);
        this.drawTo(start, end);
    }

    public void drawTo(Point start, Point end){
        robot.mouseMove((int)start.getX(), (int)start.getY());
        robot.mousePress(InputEvent.BUTTON1_MASK);
        robot.delay(100);
        robot.mouseMove((int)end.getX(), (int)end.getY());
        robot.delay(100);
        robot.mouseRelease(InputEvent.BUTTON1_MASK);
        robot.delay(100 );
    }

    public void selectColor(int num){
        selectTool(new Point(1411, 386));   // select color tool
        selectTool(new Point(448, 274));    //select color tab
        selectTool(new Point(461, 369 + (num * 18))); // select color
        selectTool(new Point(536, 612)); // press ok
    }

    public void selectTool(Point2D tool){
        robot.mouseMove((int)tool.getX(), (int)tool.getY());
        robot.mousePress(InputEvent.BUTTON1_MASK);
        robot.delay(100);
        robot.mouseRelease(InputEvent.BUTTON1_MASK);
    }

    public void drawSomething(){

        for(int i=0; i < 8; i++) {
            selectColor(i);
            drawTo( new Point(305, 208+ (20*i)),
                    new Point(1000, 208+ (20*i)));
        }
    }

}
