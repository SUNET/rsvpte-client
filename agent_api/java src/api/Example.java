package api;


import java.net.Inet4Address;
import java.net.UnknownHostException;

import unimessages.CreateReq;
import unimessages.DeleteReq;
import unimessages.StatusReq;
import communication.UniAgentConnection;
/**
 * @author Maciej Strozyk
 *
 */

public class Example {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		/*
		 * Some examples of using api interfaces implementations.
		 * 
		 */
		
		try {
			/*
			 * establishes TCP connection with Proxy Agent (ip address and port are mandatory)
			 */
			UniAgentConnection agentConnection = new UniAgentConnection(Inet4Address.getByName("150.254.160.215"), 35000);
			
			/*
			 * creates UniAgentSend object with UniAgentConnection object as a constructor parameter  
			 */
			UniAgentSendInterface agentSendInterface = new UniAgentSendInterfaceImplementation(agentConnection);
			/*
			 * Informs only if message was sent correctly to the agent (nothing about the path)
			 */
			boolean sendStatus = false;
			/*
			 * Created path id
			 */
			short pathId = 13;

			
// ***** CREATE REQ MESSAGE *****
			
			/*
			 * Creates CreateReq message with mandatory objects (path destination address and path ID)
			 */
			CreateReq createReq = new CreateReq((Inet4Address)Inet4Address.getByName("10.136.136.10"), pathId);
			
			//2. some optional, additional objects in CreateReq message (you can add some other)
			createReq.addRsvpSessionSrcAddrObj((Inet4Address)Inet4Address.getByName(("10.11.12.1")));
			//createReq.addTokenBucketRate((float)31250000.0);
			//createReq.addTokenBucketSize((float)31250000.0);
			//createReq.addPeakDataRate((float)31250000.0);
			createReq.addLspGpid((short)21);
			createReq.addLspEncodingType((byte)1);
			createReq.addLspSwitchingType((byte)1);
			createReq.addPathName("test_UNI20_path_"+pathId);
			createReq.addSenderLSPId((short)33);
			
			//New added objects (OIF UNI 2.0Eth)
			
			createReq.addEthCir((float)120.0);
			createReq.addEthCbs((float)130.0);
			createReq.addEthEir((float)140.0);
			createReq.addEthEbs((float)150.0);
			createReq.addEthSwGranuality((short)120);
			createReq.addEthIndex((byte)2);
			createReq.addEthProfile((byte)3);
			
			//...
		
			//System.out.println(createReq);
			
// ***** STATUS REQ MESSAGE *****
			
			/*
			 * Creates StatusReq message with mandatory object (tunel ID)
			 */
			StatusReq statusReq = new StatusReq(pathId);
			//System.out.println(statusReq);
			
// ***** DELETE REQ MESSAGE *****

			/*
			 * Creates DeleteReq message with mandatory object (tunel ID)
			 */
			DeleteReq deleteReq = new DeleteReq(pathId);
			//System.out.println(deleteReq);


			
/*
 * Sending messasges using Agent Interface implementation
 * (3 methods are available now)
 */

			// create path
			sendStatus = agentSendInterface.createPath(createReq);
			System.out.println("Create Req send status: send="+sendStatus);
			// some time for agent work
			Thread.sleep(3000);
			// Ask for created path status
			sendStatus = agentSendInterface.checkPathStatus(statusReq);
			System.out.println("Status Req send status: send="+sendStatus);
			Thread.sleep(3000);
			// delete path
			sendStatus = agentSendInterface.deletePath(deleteReq);
			System.out.println("Delete Req send status: send="+sendStatus);
            Thread.sleep(3000);
            // Ask for created LSP status once again
            sendStatus = agentSendInterface.checkPathStatus(statusReq);
			System.out.println("Status Req send status: send="+sendStatus);
			            

			Thread.sleep(3000);
			agentConnection.closeConnection();
			
		} 
		catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

} // Example
