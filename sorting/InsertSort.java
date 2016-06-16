
public class InsertSort {
	public static void main(String args[])
	{
		int[] arr={1,9,2,3,5,4,6,7,10,8,11};
		int i=0,j=0;
		int key=0;
		for (j=1; j<arr.length; j++)
		{
			key=arr[j];
			i=j-1;
			while(i>=0&&arr[i]>key)
			{
				arr[i+1]=arr[i];
				i=i-1;
				arr[i+1]=key;
			
								
			}
		}
		for (int k=0; k<arr.length; k++)
			{
			System.out.println(arr[k]);
			}
			
	}
}
