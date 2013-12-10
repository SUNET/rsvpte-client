package uniobjects;
/**
 * @author Maciej Strozyk
 *
 */

public class UniIntObject extends UniObject {
	
	private int dataContent;
	
	/**
	 * Constructor
	 * 
	 * @param objectType
	 * @param dataContent
	 */
	public UniIntObject(short objectType, int dataContent) {
		
		this.objectType = objectType;
		dataType = generateDataType(objectType); 
		dataSize = generateDataSize(dataType);
		this.dataContent = dataContent;
		
	}

	/**
	 * @return Returns the dataContent.
	 */
	public int getDataContent() {
		return dataContent;
	}

	/**
	 * @param dataContent The dataContent to set.
	 */
	public void setDataContent(int dataContent) {
		this.dataContent = dataContent;
	}

} // UniIntObject
