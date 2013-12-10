package uniobjects;
/**
 * @author Maciej Strozyk
 *
 */

public class UniLongObject extends UniObject {

	private long dataContent;
	
	/**
	 * Constructor
	 * 
	 * @param objectType
	 * @param dataContent
	 */
	public UniLongObject(short objectType, long dataContent) {
		
		this.objectType = objectType;
		dataType = generateDataType(objectType); 
		dataSize = generateDataSize(dataType);
		this.dataContent = dataContent;
		
	}

	/**
	 * @return Returns the dataContent.
	 */
	public long getDataContent() {
		return dataContent;
	}

	/**
	 * @param dataContent The dataContent to set.
	 */
	public void setDataContent(long dataContent) {
		this.dataContent = dataContent;
	}

} // UniLongObject
