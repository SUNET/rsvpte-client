package api;

import communication.PacketUtils;
import communication.UniAgentConnection;

import unimessages.Message;
import uniobjects.UniHeader;
/**
 * @author Maciej Strozyk
 *
 */

public class UniAgentSendInterfaceImplementation implements UniAgentSendInterface {

	UniAgentConnection agentConnection;
	
	
	public UniAgentSendInterfaceImplementation(UniAgentConnection agentConnection) {
		this.agentConnection = agentConnection;
	}

	
	/**
	 * 
	 */
	public boolean createPath(Message createReq) {
		
		UniHeader header = createReq.getHeader();
		if((header.getMessageType() != PacketUtils.REQ) || 
				(header.getMessageRequest() != PacketUtils.CREATE) )
			return false;
		
		return agentConnection.communicationSender.sendMessage(createReq);
		
	}

	
	/**
	 * 
	 */
	public boolean deletePath(Message deleteReq) {

		UniHeader header = deleteReq.getHeader();
		if((header.getMessageType() != PacketUtils.REQ) || 
				(header.getMessageRequest() != PacketUtils.DELETE) )
			return false;
		
		return agentConnection.communicationSender.sendMessage(deleteReq);
		
	}

	/**
	 * 
	 */
	public boolean checkPathStatus(Message statusReq) {

		UniHeader header = statusReq.getHeader();
		if((header.getMessageType() != PacketUtils.REQ) || 
				(header.getMessageRequest() != PacketUtils.STATUS) )
			return false;
		
		return agentConnection.communicationSender.sendMessage(statusReq);
		
	}

	
} // UniAgentSendInterfaceImplementation
