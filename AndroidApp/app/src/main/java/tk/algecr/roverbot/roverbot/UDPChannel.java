package tk.algecr.roverbot.roverbot;

import android.os.AsyncTask;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

/**
 * Created by Alex on 21/11/2015.
 */
public class UDPChannel extends AsyncTask<String, String, String> {


    private DatagramSocket clientsocket;
    private final Runnable e;
    public String mensaje[];
    public boolean error;

    private final int port;
    private final InetAddress ipServer;
    private final int portServer;

    public UDPChannel(int port,InetAddress ipServer,int portServer, Runnable callback){
        this.port = port;
        this.ipServer = ipServer;
        this.portServer = portServer;
        this.e = callback;
    }

    public void send(final String s){
        Thread sendThread = new Thread(new Runnable() {
            @Override
            public void run() {
                try{
                    if (clientsocket == null) clientsocket=new DatagramSocket(port);
                    byte[] msg = s.getBytes();
                    DatagramPacket p = new DatagramPacket(msg, s.length(), ipServer, portServer);
                    clientsocket.send(p);
                }catch(IOException e){
                    e.printStackTrace();
                }
            }
        });
        sendThread.start();

    }

    @Override
    protected String doInBackground(String... params) {
        while (true) {
            try {
                publishProgress(receiveMessage());
                if(isCancelled()) break;
            } catch (Exception e) {
                //
            }

        }
        return "";
    }

    String[] receiveMessage(){

        String[] rec_arr = null;
        try {

            if (clientsocket == null) clientsocket=new DatagramSocket(port);

            byte[] receivedata = new byte[128];

            DatagramPacket recv_packet = new DatagramPacket(receivedata, receivedata.length);
            clientsocket.receive(recv_packet);
            String rec_str = new String(recv_packet.getData()); //stringa con mesasggio ricevuto
            rec_str= rec_str.replace(Character.toString ((char) 0), "");
            rec_arr=rec_str.split("\\|");
            error = false;
            return rec_arr;

        } catch (Exception e) {
            error = true;
            e.printStackTrace();
        }
        return rec_arr;
    }


    @Override
    protected void onPostExecute(String result) {
        //
    }

    @Override
    protected void onPreExecute() {}

    @Override
    protected void onProgressUpdate(String... rec_arr) {
            this.mensaje = rec_arr;
            this.e.run();
    }
}


