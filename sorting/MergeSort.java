
public class MergeSort {
	public static void main(String args[])
	{
		int[] arr={1,9,2,3,5,4,6,7,10,8,11};
		
		Sort(arr, 0, arr.length-1);
		Merge(arr, 0, 1, arr.length-1);
		
		for (int k=0; k<arr.length-1; k++)
			{
			System.out.println(arr[k]);
			}
			
	}
	
	static void Sort(int []arr, int p, int r)
	{
		int q = 0;
		if(p<r)
		{
		q=(int) Math.floor((p+r)/2);
		Sort(arr, p, q);
		Sort(arr, q+1, r);
		Merge(arr, p,q,r);
		}
	}
	
	static void Merge(int []arr, int p,int q, int r)
	{
		int i=q-p+1;
		int j=r-q;
		int m[]=new int[i];
		int n[]=new int[j];
		for(int x=0; x<=i; x++)
			m[i]=arr[p+i-1];
		for(int y=0; y<=j; y++)
			n[j]=arr[q+j];
		
		i=0;
		j=0;
		for(int z=p; z<=r; z++)
		{
			if(m[i]<=n[j])
			{	arr[z]=m[i];
				i=i+1;		}
			else
				{
				arr[z]=n[j];
				j=j+1;
				}
		}
	
			
		
	}
}
