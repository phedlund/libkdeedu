#ifndef KALZIUMGLWIDGET_H
#define KALZIUMGLWIDGET_H
/***************************************************************************
    copyright            : (C) 2006 by Benoit Jacob
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QGLWidget>
#include <openbabel/mol.h>

#define USE_DOUBLE_PRECISION

#ifdef USE_DOUBLE_PRECISION
#define FLOAT		double
#define GLFLOAT		GLdouble
#else
#define FLOAT		float
#define GLFLOAT		GLfloat
#endif

inline float GLSQRT( float x ) { return sqrtf( x ); }
inline double GLSQRT( double x ) { return sqrt( x ); }
inline float GLSIN( float x ) { return sinf( x ); }
inline double GLSIN( double x ) { return sin( x ); }
inline float GLCOS( float x ) { return cosf( x ); }
inline double GLCOS( double x ) { return cos( x ); }
inline float GLFABS( float x ) { return fabsf( x ); }
inline double GLFABS( double x ) { return fabs( x ); }
inline void GLMULTMATRIX( const GLfloat *m ) { glMultMatrixf(m); }
inline void GLMULTMATRIX( const GLdouble *m ) { glMultMatrixd(m); }
inline void GLTRANSLATE( GLfloat x, GLfloat y, GLfloat z ) \
	{ glTranslatef( x, y, z ); }
inline void GLTRANSLATE( GLdouble x, GLdouble y, GLdouble z ) \
	{ glTranslated( x, y, z ); }

/**
 * This class represents a color in OpenGL float red-green-blue format.
 *
 * @author Benoit Jacob
 */
struct GLColor
{
	GLfloat m_red, m_green, m_blue, m_alpha;

	GLColor();
	GLColor( GLfloat red, GLfloat green, GLfloat blue,
	         GLfloat alpha = 1.0 );

	GLColor& operator=( const GLColor& other );

	/**
	 * Sets this color to be the one used by OpenGL for rendering
	 * when lighting is disabled. It just calls glColor4fv.
	 */
	inline void apply();

	/**
	 * Applies nice OpenGL materials using this color as the
	 * diffuse color while using different shades for the ambient and
         * specular colors. This is only useful if GL lighting is enabled.
	 */
	void applyAsMaterials();
};

/**
 * This template class represents a vector in 3-space. It is meant to be
 * used with T = a floating-point type.
 *
 * @author Benoit Jacob
 */

template<class T> class GLVector3
{
	public:
		T x, y, z;
		GLVector3() {}
		GLVector3( T _x, T _y, T _z)
		{ x = _x; y = _y; z = _z; }

		/**
		 * Tests whether two FLOATs are approximately equal.
		 * Recall that operator == between floating-point types
		 * is broken.
		 * returns true if abs( a - b ) <= c * precision
		 * where c = max( abs( a ), abs( b ) )
		 */
		static bool approx_equal( FLOAT a, FLOAT b, FLOAT precision );

		GLVector3<T>& operator= ( const GLVector3<T>& other )
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		/**
		 * returns the norm of the vector, that is, its length
		 */
		inline T norm() { return GLSQRT( x * x + y * y + z * z ); }

		/**
		 * normalizes the vector, that is, scales it so that its norm
		 * becomes 1.
		 */
		void normalize()
		{
			T n = norm();
			if( n == 0.0 ) return;
			x /= n;
			y /= n;
			z /= n;
		}
		
		/**
		 * Constructs two vectors v and w
		 * such that (*this, v, w) is a direct orthogonal basis.
		 * v and w are not getting normalized.
		 */
		void construct_ortho_basis_given_first_vector(
			GLVector3<T> & v, GLVector3<T> & w );
};

/**
 * This is an abstract base class for an OpenGL vertex array.
 *
 * @author Benoit Jacob
 */
class GLVertexArray
{
	private:
		int m_id;

	protected:
		GLenum m_mode;
		GLVector3<GLfloat> *m_vertexBuffer;
		GLVector3<GLfloat> *m_normalBuffer;
		unsigned int m_vertexCount;
		unsigned short *m_indexBuffer;
		unsigned int m_indexCount;
		
		bool m_isInitialized;
		
		virtual void initialize() = 0;
		virtual bool allocateBuffers();
		virtual void allocateId();
		virtual void select();

	public:
		GLVertexArray();
		virtual ~GLVertexArray();
		virtual inline void draw();
};

/**
 * This class represents and draws a sphere
 *
 * @author Benoit Jacob
 */
class GLSphere : public GLVertexArray
{
	private:
		inline unsigned short indexOfVertex(
			int strip, int column, int row);
		void computeVertex( int strip, int column, int row );

	protected:
		int m_detail;
		GLfloat m_radius;

		virtual void initialize();

	public:
		GLSphere();
		virtual ~GLSphere() {}
		virtual void setup( int detail, GLfloat radius );
		virtual void drawScaled( GLfloat radius );
};

/**
 * This class represents and draws a cylinder
 *
 * @author Benoit Jacob
 */
class GLCylinder : public GLVertexArray
{
	protected:
		int m_faces;
		GLfloat m_radius;

		virtual void initialize();

	public:
		GLCylinder();
		virtual ~GLCylinder() {}
		virtual void setup( int detail, GLfloat radius );
		virtual inline void draw();
};

/**
 * This class displays the 3D-view of a molecule
 * 
 * @author Benoit Jacob
 */
class KalziumGLWidget : public QGLWidget
{
	Q_OBJECT

	protected:
		/**
		 * The geometric model of the sphere (used for atoms).
		 */
		GLSphere m_sphere;

		/**
		 * The geometric model of the cylinder (used for bonds).
		 */
		GLCylinder m_cylinder;
		
		/**
		 * equals true if the user is currently dragging (rotating)
		 * the view
		 */
		bool m_isDragging;

		QPoint m_lastDraggingPosition;


		/**
		 * Stores the rotation that is applied to the model.
		 */
		GLFLOAT m_RotationMatrix[16];

		/**
		 * The molecule which is displayed
		 */
		OpenBabel::OBMol* m_molecule;

		/**
		 * approximate radius of the molecule
		 */
		GLFLOAT m_molRadius;

		/**
		 * length of the shortest bond in the molecule
		 */
		GLFLOAT m_molMinBondLength;

		/**
		 * length of the longest bond in the molecule
		 */
		GLFLOAT m_molMaxBondLength;

		/**
		 * The coefficient set by the user, determining the
		 * radius of atoms.
		 * WARNING: its meaning has just changed! (june 17)
		 * Now the actual radius is proportional to
		 * m_atomRadiusCoeff.
		 */
		float m_atomRadiusCoeff;

		/**
		 * The coefficient set by the user, determining the
		 * radius (that is, half the thickness) of bonds.
		 */
		float m_bondRadiusCoeff;

		/**
		 * The detail-grade from 0 to 2.
		 */
		int m_detail;

		/**
		 * Set this to true to enable the fog effect
		 */
		bool m_useFog;

		/**
		 * The selected atom
		 */
		OpenBabel::OBAtom* m_selectedAtom;

		/**
		 * The style in which the atoms are rendered.
		 */
		enum AtomStyle
		{
			ATOM_DISABLED,
			ATOM_SPHERE
		} m_atomStyle;

		/**
		 * The style in which the bonds are rendered.
		 */
		enum BondStyle
		{
			BOND_DISABLED,
			BOND_LINE,
			BOND_CYLINDER_GRAY,
			BOND_CYLINDER_BICOLOR
		} m_bondStyle;

		/**
		 * Some style presets
		 */
		enum StylePreset
		{
			PRESET_LINES,
			PRESET_STICKS,
			PRESET_SPHERES_AND_GRAY_BONDS,
			PRESET_SPHERES_AND_BICOLOR_BONDS,
			PRESET_BIG_SPHERES
		};

	public:
		/**
		 * Constructor
		 */
		KalziumGLWidget( QWidget *parent = 0 );

		/**
		 * Destructor
		 */
		virtual ~KalziumGLWidget();

		/**
		 * returns a pointer to the molecule being worked on
		 */
		inline OpenBabel::OBMol* molecule () const
			{ return m_molecule; }

	public slots:
		/**
		 * sets the molecule which will be displayed
		 * @param molecule the molecule to render
		 */
		void slotSetMolecule( OpenBabel::OBMol* molecule );
		
		/**
		 * Sets the detail-grade in a range from 0 to 2
		 *  @param detail the detail-grade of the rendering. 0 is low, 2 is high
		 */
		void slotSetDetail( int detail );

		/**
		 * Chooses the style of rendering among some presets
		 * @param stylePreset the wanted style preset
		 */
		/**
		 * Chooses the style of rendering among some presets
		 * @param stylePreset the wanted style preset
		 */
		void slotChooseStylePreset( int stylePreset ){
			ChooseStylePreset( (StylePreset) stylePreset );
		}

		/**
		 * The atom @p atom was selected by the user
		 */
		void slotAtomSelected( OpenBabel::OBAtom* atom );

	protected:
		/**
		 * This method initializes OpenGL. Automatically called by Qt
		 */
		virtual void initializeGL();

		/**
		 * This method does the painting. Automatically called by Qt
		 */
		virtual void paintGL();

		/**
		 * This method is called by Qt whenever the widget is resized.
		 */
		virtual void resizeGL( int width, int height );

		virtual void mousePressEvent( QMouseEvent * event );
		virtual void mouseReleaseEvent( QMouseEvent * event );
		virtual void mouseMoveEvent( QMouseEvent * event );

		/**
		 * This method is called by slotSetMolecule. It prepares the
		 * molecule for rendering, and computes some useful data about
		 * it.
		 */
		void prepareMoleculeData();
		
		/**
		 * This method draws a sphere
		 * @param x
		 * @param y
		 * @param z
		 * @param red
		 * @param green
		 * @param blue
		 */

		virtual void drawSphere( 
				GLFLOAT x, 
				GLFLOAT y, 
				GLFLOAT z, 
				GLfloat radius,
				GLColor &color );

		/**
		 * This method draws a bond
		 * @param x
		 * @param y
		 * @param z
		 * @param radius
		 * @param red
		 * @param green
		 * @param blue
		 */

		virtual void drawBond( GLFLOAT x1, GLFLOAT y1, GLFLOAT z1,
			GLFLOAT x2, GLFLOAT y2, GLFLOAT z2,
			GLColor &color );

		/**
		 * returns the radius ( = half-thickness ) with which the
		 * bonds are drawn
		 */
		inline GLFLOAT bondRadius();

		/**
		 * returns the radius with which the atoms are drawn
		 * (currently all atoms are drawn with the same radius.
		 * when we'll have a better model taking van der Waals radii
		 * into account, we'll remove this member).
		 */
		inline GLFLOAT atomRadius();

		/**
		 * Chooses the style of rendering among some presets
		 * @param stylePreset the wanted style preset
		 */
		virtual void ChooseStylePreset( StylePreset stylePreset );

		/**
		 * returns the color which a given atom should be painted
		 */
		GLColor& getAtomColor( OpenBabel::OBAtom* atom );

		/**
		 * recomputes the geometry of the geometric objects ( sphere,
		 * cylinder ).
		 */
		virtual void setupObjects();
};


#endif // KALZIUMGLWIDGET_H
