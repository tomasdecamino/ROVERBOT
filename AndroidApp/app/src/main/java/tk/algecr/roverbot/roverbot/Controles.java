package tk.algecr.roverbot.roverbot;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;

/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 */
public class Controles extends AppCompatActivity {
    private final int portServer = 2390;
    private final int UPDMAXPORT = 5000;
    private final int UDPMINPORT = 2000;

    UDPChannel receiveSocket;
    private final int localPort;
    private int velI;
    private int velD;

    public Controles(){
        this.localPort =  (new Random()).nextInt((UPDMAXPORT-UDPMINPORT)+1)+UDPMINPORT;

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_controles);
        velI = velD = 0;
        startReceiveUdp();
        doSomethingRepeatedly();
    }

    public void AD(View v){
        ++velD;
        ++velI;
    }

    public void AT(View v){
        --velD;
        --velI;
    }

    public void IZ(View v){
        ++velD;
        --velI;
    }

    public void DE(View v){
        --velD;
        ++velI;
    }

    public void stop(View v){
        velD = velI = 0;
    }

    private void doSomethingRepeatedly() {
        Timer timer = new Timer();
        timer.scheduleAtFixedRate( new TimerTask() {
            public void run() {
                try{
                    String s = "";
                    s += velI;
                    s += ',';
                    s += velD;
                    receiveSocket.send(s);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }, 0, 100);
    }

    void startReceiveUdp() {
        if (receiveSocket==null) {
            try {
                receiveSocket=new UDPChannel(localPort,InetAddress.getByName("192.168.4.1"),portServer,new Runnable() {
                    @Override
                    public void run() {
                        if(!receiveSocket.error && receiveSocket.mensaje[0] != null) {
                            TextView t = (TextView) findViewById(R.id.debug);
                            t.setText(receiveSocket.mensaje[0]);
                        }
                    }
                });
                receiveSocket.execute("");
            } catch (UnknownHostException e) {
                e.printStackTrace();
            }

        }
    }

    void stopReceiveUdp() {
        if (receiveSocket!=null) receiveSocket.cancel(true);
    }
}