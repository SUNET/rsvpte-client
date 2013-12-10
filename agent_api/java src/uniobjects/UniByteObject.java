package uniobjects;
/**
 * @author Maciej Strozyk
 *
 */

public class UniByteObject extends UniObject {

	private byte dataContent;
	
	/**
	 * Constructor
	 * 
	 * @param objectType
	 * @param dataContent
	 */
	public UniByteObject(short objectType, byte dataContent) {
		
		this.objectType = objectType;
		dataType = generateDataType(objectType); 
		dataSize = generateDataSize(dataType);
		this.dataContent = dataContent;
		
	}

	/**
	 * @return Returns the dataContent.
	 */
	public byte getDataContent() {
		return dataContent;
	}

	/**
	 * @param dataContent The dataContent to set.
	 */
	public void setDataContent(byte dataContent) {
		this.dataContent = dataContent;
	}
	

} // UniByteObject
