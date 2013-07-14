// A sample header with doccomments for kdoc.
// Sirtaj S. Kang (taj@kde.org), Dec 1997.

/**
 * This is my class. It is a very simple class.
 *
 * This is the documentation for my class. . Notice that there
 * is nothing between this documentation section and the declaration for
 * the class.
 *
 * @short My Simple Class
 * @author Sirtaj S. Kang (taj@kde.org)
 * @version 0.1alpha
 * @see KApplication::getConfig
 */
class MyClass : public MyBaseClass
{
public:
        // below is an empty doc tag.

        /**
        */
        void myMethod();

        /**
         * I am a documented method.
         * Build your own class documentation today.
         *
         * Since these sentences are separated from the rest by atleast
         * one blank line, this will be a paragraph by itself. This
	 * also demonstrates an inline reference to @ref KApplication.
         *
         * @see anotherClass#someOtherMethod
         * @see aThirdClass
         *
         * @return non-zero on success, zero otherwise.
         * @param parA  A parameter that isnt used.
	 *		
         * @param parB  Another parameter.
         */
        int myCuteMethod( int parA, const char *parB );

	/** 
	 * Not part of the interface.
	 * @deprecated
	 */
	void anotherMethod() const;

	class nads {
		12312 1234;
		wqrqwr() { }
		int a;
	};

	typedef int temperature;

protected:
	/** 
	 * This variable is unused.
	 * @see #myCuteMethod
	 */
	int _unused;
};
