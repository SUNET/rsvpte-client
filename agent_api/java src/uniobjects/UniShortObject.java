package uniobjects;
/**
 * @author Maciej Strozyk
 *
 */

public class UniShortObject extends UniObject {

	private short dataContent;
	
	/**
	 * Constructor
	 * 
	 * @param objectType
	 * @param dataContent
	 */
	public UniShortObject(short objectType, short dataContent) {
		
		this.objectType = objectType;
		dataType = generateDataType(objectType); 
		dataSize = generateDataSize(dataType);
		this.dataContent = dataContent;
		
	}

	/**
	 * @return Returns the dataContent.
	 */
	public short getDataContent() {
		return dataContent;
	}

	/**
	 * @param dataContent The dataContent to set.
	 */
	public void setDataContent(short dataContent) {
		this.dataContent = dataContent;
	}

} // UniShortObject
