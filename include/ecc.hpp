#ifndef ECC_HPP
#define ECC_HPP

#include "bigint.hpp"
#include <iostream>

#define USE_CURVE_P256
// #define USE_CURVE_SECP256K1
// #define USE_CURVE_P521

/**
 * @struct CurveParameters
 * @brief Holds the parameters of an elliptic curve.
 *
 * This structure stores the essential parameters that define an elliptic curve over a finite field.
 * These include the coefficients 'a' and 'b' of the elliptic curve equation y^2 = x^3 + ax + b,
 * the prime 'p' defining the finite field size, the coordinates 'Gx' and 'Gy' of the generator
 * point, and 'n' which is the order of the group generated by the generator point.
 */
struct CurveParameters {
    BigInt a;  ///< Coefficient a of the elliptic curve equation.
    BigInt b;  ///< Coefficient b of the elliptic curve equation.
    BigInt p;  ///< Prime number defining the field size.
    BigInt Gx; ///< x-coordinate of the generator point.
    BigInt Gy; ///< y-coordinate of the generator point.
    BigInt n;  ///< Order of the group generated by the generator point.
};

/**
 * @class Ecc_Point
 * @brief Represents a point on an elliptic curve.
 *
 * The Ecc_Point class encapsulates a point on an elliptic curve defined over a finite field.
 * It provides functionalities for elliptic curve arithmetic like point addition,
 * negation, and scalar multiplication.
 */
class Ecc_Point {
public:
    bool isInfinity;
    /**
     * @brief Default constructor to initialize an Ecc_Point with BigInt coordinates. Initializes with base points as x and y coordinate.
     * @param x The x-coordinate of the Ecc_Point.
     * @param y The y-coordinate of the Ecc_Point.
     */
    Ecc_Point() : xCoord(BigInt(static_cast<unsigned long int>(0))), yCoord(BigInt(static_cast<unsigned long int>(0))), isInfinity(true) {}
    
    /**
     * @brief Constructor to initialize an Ecc_Point with BigInt coordinates.
     * @param x The x-coordinate of the Ecc_Point.
     * @param y The y-coordinate of the Ecc_Point.
     */
    Ecc_Point(const BigInt& x, const BigInt& y) : xCoord(x), yCoord(y), isInfinity(false) {
        setCurveParameters();
    }

    /**
     * @brief Copy constructor.
     * @param other The Ecc_Point to copy.
     */
    Ecc_Point(const Ecc_Point& other) 
    : xCoord(other.xCoord), yCoord(other.yCoord), isInfinity(other.isInfinity), curveParams(other.curveParams) {}

    /**
     * @brief Assignment operator.
     * @param other The Ecc_Point to assign.
     * @return Reference to this Ecc_Point after assignment.
     */
    Ecc_Point& operator=(const Ecc_Point& other) {
        if (this != &other) {
            xCoord = other.xCoord;
            yCoord = other.yCoord;
            isInfinity = other.isInfinity;
            curveParams = other.curveParams;
        }
        return *this;
    }

    /**
     * @brief Get the x-coordinate of the Ecc_Point.
     * @return The x-coordinate.
     */
    const BigInt& getX() const { return xCoord; }

    /**
     * @brief Get the y-coordinate of the Ecc_Point.
     * @return The y-coordinate.
     */
    const BigInt& getY() const { return yCoord; }

    const BigInt& getP() const {
        return curveParams.p;
    }

    /**
     * @brief Set the x-coordinate of the Ecc_Point.
     * @param x The new x-coordinate.
     */
    void setX(const BigInt& x) { xCoord = x; }

    /**
     * @brief Set the y-coordinate of the Ecc_Point.
     * @param y The new y-coordinate.
     */
    void setY(const BigInt& y) { yCoord = y; }

    /**
     * @brief Prints the coordinates of the Ecc_Point.
     *
     * This function displays the x and y coordinates of the Ecc_Point. If the point
     * is at infinity, it prints a message indicating so. This is useful for debugging
     * and verifying the values of the Ecc_Point during computations.
     */
    void print() const {
        if (isInfinity) {
            std::cout << "Point at Infinity" << std::endl;
        } else {
            std::cout << "Ecc_Point Coordinates:" << std::endl;
            std::cout << "x = " << xCoord.toString(16) << std::endl;
            std::cout << "y = " << yCoord.toString(16) << std::endl;
        }
    }
    
    /**
     * @brief Overloads the + operator for adding two Ecc_Points.
     *
     * This function implements the addition of two points on an elliptic curve.
     * Given two points P = (x1, y1) and Q = (x2, y2), it computes the resulting
     * point R = P + Q = (x3, y3) using elliptic curve addition formulas.
     *
     * The addition is defined as follows:
     * 1. If P = Q, then the point doubling formula is applied.
     * 2. If P ≠ Q, the slope λ = (y2 - y1) / (x2 - x1) is computed, followed by
     *    x3 = λ² - x1 - x2 and y3 = λ(x1 - x3) - y1.
     * 3. Special cases, like handling the point at infinity, are also considered.
     *
     * Operations are performed under modulo p arithmetic for a prime field.
     *
     * @param other The Ecc_Point to add to this point.
     * @return Ecc_Point representing the sum of this point and the other point.
     */
    Ecc_Point operator+(const Ecc_Point& other) const;

    /**
     * @brief Overloads the - operator to negate this Ecc_Point.
     *
     * Negating a point P = (x, y) on the elliptic curve results in the point -P = (x, -y).
     * In the context of modulo arithmetic for a prime field, -y is computed as p - y.
     *
     * This operation is useful for elliptic curve point subtraction, as P - Q is equivalent
     * to P + (-Q).
     *
     * @return Ecc_Point representing the negation of this point on the elliptic curve.
     */
    Ecc_Point operator-() const ;


    /**
     * @brief Overloads the * operator for scalar multiplication of this Ecc_Point.
     *
     * Scalar multiplication is a fundamental operation in elliptic curve cryptography.
     * It computes kP for a point P on the curve and a scalar k. This operation is
     * equivalent to adding P to itself k times.
     *
     * The scalar multiplication is performed using the "double-and-add" method, which
     * is efficient and reduces the number of elliptic curve operations needed.
     *
     * @param scalar The BigInt scalar to multiply this point by.
     * @return Ecc_Point resulting from the scalar multiplication of this point by the scalar.
     */
    Ecc_Point operator*(const BigInt& scalar) const;


    /**
     * @brief Overloads the == operator to compare two Ecc_Points.
     *
     * This function checks if two points on the elliptic curve are equal. Two points
     * P = (x1, y1) and Q = (x2, y2) are considered equal if and only if x1 ≡ x2 and
     * y1 ≡ y2. This comparison is crucial in many cryptographic algorithms to determine
     * if two points are the same.
     *
     * The equality check is performed under modulo p arithmetic for a prime field.
     *
     * @param other The Ecc_Point to compare with this point.
     * @return True if the points are equal (both x and y coordinates match), false otherwise.
     */
    bool operator==(const Ecc_Point& other) const;


private:
    CurveParameters curveParams;
    BigInt xCoord; ///< The x-coordinate of the Ecc_Point.
    BigInt yCoord; ///< The y-coordinate of the Ecc_Point.


     /**
     * @brief Sets the parameters of the elliptic curve based on the selected curve type.
     *
     * This function initializes the curve parameters (a, b, p, Gx, Gy, n) of the Ecc_Point
     * object based on a predefined set of well-known elliptic curves. The specific curve
     * is selected using preprocessor directives at compile time. This function is invoked
     * during the construction of an Ecc_Point object to ensure it is configured with the
     * correct parameters for the chosen elliptic curve.
     *
     * @note The curve parameters must be predefined and correctly set for the specified
     *       elliptic curve. If no curve is defined, a compile-time error is generated.
     */

    void setCurveParameters() {
        #ifdef USE_CURVE_P256
            curveParams.a = BigInt("ffffffff00000001000000000000000000000000fffffffffffffffffffffffc",16);
            curveParams.b = BigInt("5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b",16);
            curveParams.p = BigInt("ffffffff00000001000000000000000000000000ffffffffffffffffffffffff",16);
            curveParams.Gx = BigInt("6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296",16);
            curveParams.Gy = BigInt("4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5",16);
            curveParams.n = BigInt("ffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551",16);
        #elif defined USE_CURVE_SECP256K1
            curveParams.a = BigInt("0",16);
            curveParams.b = BigInt("7",16);
            curveParams.p = BigInt("fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f",16);
            curveParams.Gx = BigInt("79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798",16);
            curveParams.Gy = BigInt("483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8",16);
            curveParams.n = BigInt("fffffffffffffffffffffffe26f2fc170f69466a74defd8d",16);
        #elif defined USE_CURVE_P521
            curveParams.a = BigInt("01fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffc",16);
            curveParams.b = BigInt("0051953eb9618e1c9a1f929a21a0b68540eea2da725b99b315f3b8b489918ef109e156193951ec7e937b1652c0bd3bb1bf073573df883d2c34f1ef451fd46b503f00",16);
            curveParams.p = BigInt("01ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",16);
            curveParams.Gx = BigInt("00c6858e06b70404e9cd9e3ecb662395b4429c648139053fb521f828af606b4d3dbaa14b5e77efe75928fe1dc127a2ffa8de3348b3c1856a429bf97e7e31c2e5bd66",16);
            curveParams.Gy = BigInt("011839296a789a3bc0045c8a5fb42c7d1bd998f54449579b446817afbd17273e662c97ee72995ef42640c550b9013fad0761353c7086a272c24088be94769fd16650",16);
            curveParams.n = BigInt("01fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffa51868783bf2f966b7fcc0148f709a5d03bb5c9b8899c47aebb6fb71e91386409",16);
        #else
            #error "No elliptic curve defined"
        #endif
    }


    /**
     * @brief Doubles this Ecc_Point on the elliptic curve.
     *
     * This function implements the point doubling operation for elliptic curves
     * in Weierstrass form. It calculates 2P = (x', y') for the current point P = (x, y).
     * The doubling is done using the formulas: λ = (3x² + a) / 2y, x' = λ² - 2x,
     * and y' = λ(x - x') - y, where 'a' is the curve parameter and operations are
     * performed under modulo p arithmetic, with p being the prime order of the field.
     * The point P is represented by the member variables xCoord and yCoord.
     *
     * @note Assumes the point is not at infinity and y-coordinate is not zero. 
     * Additional handling is required for these special cases.
     *
     * @return Ecc_Point representing the doubled point 2P on the elliptic curve.
     */
    Ecc_Point doublePoint() const;
};

#endif // ECC_HPP
