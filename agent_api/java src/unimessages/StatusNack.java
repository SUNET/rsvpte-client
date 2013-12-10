package unimessages;

import uniobjects.UniHeader;
/**
 * @author Maciej Strozyk
 *
 */

public class StatusNack extends Message {
	
	/**
	 * 
	 * Constructor
	 *
	 */
	public StatusNack() {
		
		super(UniHeader.NACK, UniHeader.STATUS);
	}

} //StatusNack
