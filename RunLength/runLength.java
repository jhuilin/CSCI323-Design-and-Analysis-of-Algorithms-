import java.io.*;
import java.util.*;
public class runLength {
	public static void main(String[] args) throws IOException
	{
		int col=0,row=0,min=0,max=0;
		int next=0;
		int current =0;
		try 
		{
				Scanner key = new Scanner(new File(args[0]));
				FileWriter output = new FileWriter(new File(args[1]));
				if(key.hasNextInt())
					row=key.nextInt();
				if(key.hasNextInt())
					col=key.nextInt();
				if(key.hasNextInt())
					max=key.nextInt();
				if(key.hasNextInt())
					min=key.nextInt();
				output.write("header:"+row+" "+col+" "+max+" "+min);
				output.write("\n");
				int r=0;
			while(r<row) 
			{
				int c=0;
				int count=0;
				current=key.nextInt();
				
				output.write(r+","+c+","+current);
				count++;
			
			while(c!=(col-1)) 
			{
				c++;
				
				next=key.nextInt();
				
				if(next==current)
					count++;
				else
				{
					output.write(","+count);
					output.write("\n");
					current=next;
					count=1;
					output.write(r+","+c+","+current);
				}
			}
			output.write(","+count);
			output.write("\n");
			r++;
		}
			key.close();
			output.close();
	}
		catch(IOException e) {
			System.out.print("Error");
		}
}
}
