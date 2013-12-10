package unimessages;

import uniobjects.UniHeader;
/**
 * @author Maciej Strozyk
 *
 */

public class StatusAck extends Message {

	/**
	 * 
	 * Constructor
	 *
	 */
	public StatusAck() {
		
		super(UniHeader.ACK, UniHeader.STATUS);
	}

} //StatusAck
