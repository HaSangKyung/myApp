
public class HeapSort {
	
	public static void main(String args[])
	{
		int[] arr={1,9,2,3,5,4,6,7,10,8,11};
		int heap_Size=arr.length;	
		
		Buile_Heap(arr);
		for(int i=arr.length; i<=2; i--)
			{
			int swap = arr[1];
			arr[1]=arr[i];
			arr[i]=swap;
			
			
			}
		heap_Size=heap_Size-1;
			max_heapify(arr, 1);
		for (int k=0; k<arr.length; k++)
		{
		System.out.println(arr[k]);
		}
		
			
	}
	
	static void Buile_Heap(int []arr)
	{
		
		for(int i=(int)Math.floor(arr.length/2); i>=0; i--)
			max_heapify(arr,i);
	};
	
	static void max_heapify(int []arr, int i)
	{
		
		int l=2*i+1;
		int r=2*i+2;
		
		int heap_Size = 5;
		int largest;
		
		if(l<=heap_Size&&arr[l]>arr[i])
			largest=l;
		else largest=i;
		if(r<=heap_Size&&arr[r]>arr[largest])
			largest=r;
		if(largest!=i)
		{
			int swap = arr[i];
			arr[i]=arr[largest];
			arr[largest]=swap;
			
			max_heapify(arr, largest);
		
		}
			
	}
}
