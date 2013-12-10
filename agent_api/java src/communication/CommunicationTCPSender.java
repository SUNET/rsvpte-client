package communication;



import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import unimessages.Message;

/**
 * @author Maciej Strozyk
 *
 */
public class CommunicationTCPSender {
    
    private Socket communicationSocket = null;
    private OutputStream out = null; 
    private InputStream in = null; 

    public CommunicationTCPSender(InetAddress serverAddress, int serverPort) {
        
        try {
        	communicationSocket = new Socket(serverAddress, serverPort);
            out = communicationSocket.getOutputStream();
            in = communicationSocket.getInputStream();
        } catch (UnknownHostException e) {
           System.err.println("Unknown host");
       } catch (IOException e) {
           System.err.println("I/O error" +e);
       }
    }

    private boolean sendPacket(byte[] data) {
    	try {
            out.write(data);
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
        return true;
        
    }

    public boolean sendMessage(Message messagePacket) {
    	
    	byte[] dataPacketBuf = PacketUtils.packPacketIntoArray(messagePacket);
    	return sendPacket(dataPacketBuf);
    	
    }

    public InputStream getIn() {
        return in;
    }
    public void setIn(InputStream in) {
        this.in = in;
    }
    public OutputStream getOut() {
        return out;
    }
    public void setOut(OutputStream out) {
        this.out = out;
    }
    
    public Socket getCommunicationSocket() {
        return communicationSocket;
    }
    public void setCommunicationSocket(Socket communicationSocket) {
        this.communicationSocket = communicationSocket;
    }
    
} // CommunicationTCPSender
