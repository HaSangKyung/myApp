
public class BubbleSort {
	public static void main(String args[])
	{
		int[] arr={1,9,2,3,5,4,6,7,10,8,11};
		int i=0,j=0,temp=0;
		
		for (i=0; i<arr.length; i++)
		{	for(j=0; j<=i-1; j++)
				{
					if(arr[j]>arr[j+1])
					{
					temp=arr[j];
					arr[j]=arr[j+1];
					arr[j+1]=temp;
					}
				}		
			
		}
		for (int k=0; k<arr.length; k++)
			{
			System.out.println(arr[k]);
			}
			
	}
}
