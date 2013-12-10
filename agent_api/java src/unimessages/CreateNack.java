package unimessages;

import uniobjects.UniHeader;
/**
 * @author Maciej Strozyk
 *
 */

public class CreateNack extends Message {
	
	/**
	 * 
	 * Constructor
	 *
	 * @param tunnelId
	 */
	public CreateNack(short tunnelId) {
		
		super(UniHeader.NACK, UniHeader.CREATE);

		addTunnelId(tunnelId);
	
	}

} //CreateNack
