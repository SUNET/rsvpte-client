package uniobjects;
/**
 * @author Maciej Strozyk
 *
 */

public class UniFloatObject extends UniObject {

	private float dataContent;
	
	/**
	 * Constructor
	 * 
	 * @param objectType
	 * @param dataContent
	 */
	public UniFloatObject(short objectType, float dataContent) {
		
		this.objectType = objectType;
		dataType = generateDataType(objectType); 
		dataSize = generateDataSize(dataType);
		this.dataContent = dataContent;
		
	}

	/**
	 * @return Returns the dataContent.
	 */
	public float getDataContent() {
		return dataContent;
	}

	/**
	 * @param dataContent The dataContent to set.
	 */
	public void setDataContent(float dataContent) {
		this.dataContent = dataContent;
	}

} // UniFloatObject
