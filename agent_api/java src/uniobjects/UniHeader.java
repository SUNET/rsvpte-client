package uniobjects;

import communication.PacketConstants;
/**
 * @author Maciej Strozyk
 *
 */

public class UniHeader implements PacketConstants {


	
	short messageId;
	byte messageType;
	byte messageRequest;
	byte messageObjectsCounter;

	
	/**
	 * Constructor
	 * @param messageId
	 * @param messageType
	 * @param messageRequest
	 */
	public UniHeader(short messageId, 
					 byte messageType,	
					 byte messageRequest) {

		
		//System.out.println(messageId+" "+messageType+" "+messageRequest);
		this.messageId = messageId;
		this.messageType = messageType;
		this.messageRequest = messageRequest;
		this.messageObjectsCounter = 0;

	}
	
	public void updateMessageObjectsCounter(byte objectsCounter) {
		messageObjectsCounter = objectsCounter;
	}
	
	public int  getHeaderSize() {
		return PacketConstants.HEADER_SIZE;
	}

	/**
	 * @return Returns the messageId.
	 */
	public short getMessageId() {
		return messageId;
	}

	/**
	 * @param messageId The messageId to set.
	 */
	public void setMessageId(short messageId) {
		this.messageId = messageId;
	}

	/**
	 * @return Returns the messageObjectsCounter.
	 */
	public byte getMessageObjectsCounter() {
		return messageObjectsCounter;
	}

	/**
	 * @param messageObjectsCounter The messageObjectsCounter to set.
	 */
	public void setMessageObjectsCounter(byte messageObjectsCounter) {
		this.messageObjectsCounter = messageObjectsCounter;
	}

	/**
	 * @return Returns the messageRequest.
	 */
	public byte getMessageRequest() {
		return messageRequest;
	}

	/**
	 * @param messageRequest The messageRequest to set.
	 */
	public void setMessageRequest(byte messageRequest) {
		this.messageRequest = messageRequest;
	}

	/**
	 * @return Returns the messageType.
	 */
	public byte getMessageType() {
		return messageType;
	}

	/**
	 * @param messageType The messageType to set.
	 */
	public void setMessageType(byte messageType) {
		this.messageType = messageType;
	}
	

} //UniHeader
