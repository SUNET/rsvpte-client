package communication;


import java.io.IOException;
import java.io.InputStream;

import api.UniAgentReceiveInterface;
import api.UniAgentReceiveInterfaceImplementation;

import unimessages.Message;

/**
 * @author Maciej Strozyk
 *
 */
public class CommunicationTCPReceiver extends Thread implements PacketConstants {

    private byte[] dataBuffer = new byte[RECEIVER_BUFFER_SIZE];
    private InputStream in;
    private int dataBufferSize;
    private boolean runStatus = true;
    private UniAgentReceiveInterface uniAgentReceiveInterface = new UniAgentReceiveInterfaceImplementation(); 
    
    
    public CommunicationTCPReceiver(InputStream in) {
        this.in = in;
    }
    
    public void run() {
        //System.out.println("TCP packet receiver started");
        while(runStatus) {
            
            try {
                dataBufferSize = in.read(dataBuffer);
                decodeReceivedMessage(dataBuffer);
            } catch (IOException e) {
            }
        }
    }


    private void decodeReceivedMessage(byte[] dataArray) {
    	
    	Message message = PacketUtils.unpackMessageArray(dataArray);
    	//System.out.println(dumpBytes(dataArray,dataBufferSize));
    	uniAgentReceiveInterface.serviceReceivedMessage(message);
    }
    

    public synchronized void endReceiver() {
        runStatus = false;
        try {
            in.close();
        } catch (IOException e) {
        }
    }
    

    public InputStream getIn() {
        return in;
    }
    
    
    public void setIn(InputStream in) {
        this.in = in;
    }

    
    //debug method
    private String dumpBytes(byte[] dataBuffer, int dataBufferSize) {
        StringBuffer ret = new StringBuffer(dataBufferSize);
        for (int i = 0; i < dataBufferSize; i++) {
            String hex = Integer.toHexString(0x0100 + (dataBuffer[i] & 0x00FF)).substring(1);
            ret.append((hex.length() < 2 ? "0" : "") + hex);
        }
        return ret.toString();
    }

} // CommunicationTCPReceiver
