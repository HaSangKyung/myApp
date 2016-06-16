
public class QuickSort {
	public static void main(String args[])
	{
		int[] arr={1,9,2,3,5,4,6,7,10,8,11};
		
		quicksort(arr, 0, arr.length-1);
		partition(arr, 0, arr.length-1);
		
		for (int k=0; k<arr.length; k++)
			{
			System.out.println(arr[k]);
			}
			
	}
	
	static void quicksort(int []arr, int p, int r)
	{
		
		int q = 0;
		if(p<r)
		{
		q=partition(arr,p,r);
		quicksort(arr,p,q-1);
		quicksort(arr,q+1,r);
		}
	}
	
	static int partition(int []arr, int p,int r)
	{
		int x= arr[r];
		int i= p-1;
		for(int j=p;j<=r-1;j++)
		{
			if(arr[j]<=x)
			{
				i=i+1;
				int temp=arr[i];
				arr[i]=arr[j];
				arr[j]=temp;
			}
		}
		int temp=arr[i+1];
		arr[i+1]=arr[r];
		arr[r]=temp;
		
		return i+1;
		
	}
}
