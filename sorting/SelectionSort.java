
public class SelectionSort {
	public static void main(String args[])
	{
		int[] arr={1,9,2,3,5,4,6,7,10,8,11};
		int i=0,j=0,smallest=0;
		
		for(i=1; i<arr.length-1;i++)
		{
			smallest=i;
			for(j=i+1; j<arr.length; j++)
			{
				if(arr[smallest]>arr[j])
					smallest=j;
			}
			if(smallest!=i)
			{
				int temp=arr[i];
				arr[i]=arr[smallest];
				arr[smallest]=temp;
				
			}
		}
					
		for (int k=0; k<arr.length; k++)
			{
			System.out.println(arr[k]);
			}
			
	}
}

