package api;

import unimessages.Message;
/**
 * @author Maciej Strozyk
 *
 */

public interface UniAgentSendInterface {

	
	public boolean createPath(Message createReq);

	public boolean deletePath(Message deleteReq);

	public boolean checkPathStatus(Message statusReq);
	
	// TODO
	// other methods...

	
} //UniAgentSendInterface
