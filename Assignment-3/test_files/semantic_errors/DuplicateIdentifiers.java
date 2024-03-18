public class DuplicateIdentifiers {
    public static void main(String[] a) {
        System.out.println(new MyClass().Pen(1, 2));

    }
}

class DuplicateIdentifiers {
    int a;
    int a;// @error - semantic (Already Declared variable: 'a')

    public boolean func() {
        return true;
    }

    public int func() {
        return 1;
    }// @error - semantic (Already Declared Function: 'func')
}// @error - semantic (Already Declared Class: 'DuplicateIdentifiers')

class MyClass {
    boolean b;
    boolean b;// @error - semantic (Already Declared variable: 'b')

    public int Pen(int param, int param) {// @error - semantic (Already Declared parameter: 'param')
        return 1;
    }

    public int Pen2(int param) {
        int param; // @error - semantic (Already Declared parameter: 'param')
        return 1;
    }
}