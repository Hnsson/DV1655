public class MissingMethodBraces {
    public static void main(String[] args) 
        int number; // @error - syntax (Missing opening and closing brace for the main method)
        System.out.println(number);
}