package uniobjects;

import java.net.Inet4Address;
/**
 * @author Maciej Strozyk
 *
 */

public class UniInetAddrObject extends UniObject {

	private Inet4Address dataContent;
	
	/**
	 * Constructor
	 * 
	 * @param objectType
	 * @param dataContent
	 */
	public UniInetAddrObject(short objectType, Inet4Address dataContent) {
		
		this.objectType = objectType;
		dataType = generateDataType(objectType); 
		dataSize = generateDataSize(dataType);
		this.dataContent = dataContent;
		
	}

	/**
	 * @return Returns the dataContent.
	 */
	public Inet4Address getDataContent() {
		return dataContent;
	}

	/**
	 * @param dataContent The dataContent to set.
	 */
	public void setDataContent(Inet4Address dataContent) {
		this.dataContent = dataContent;
	}

} // UniInetAddrObject
