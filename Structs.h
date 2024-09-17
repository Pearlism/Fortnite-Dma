#pragma once
#include <vector>
#include <array>
#include <cmath>
#include <iostream>
#include <unordered_map>
#define M_PI 3.14159265358979323846264338327950288419716939937510

typedef struct _D3DMATRIX {
    union {
        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;

        };
        float m[4][4];
    };
} D3DMATRIX;

struct FVector
{
public:
    double X;
    double Y;
    double Z;


    FVector() : X(0), Y(0), Z(0) {}
    FVector(double InX, double InY, double InZ) : X(InX), Y(InY), Z(InZ) {}


    FVector operator+(const FVector& Other) const
    {
        return FVector(X + Other.X, Y + Other.Y, Z + Other.Z);
    }

    FVector operator-(const FVector& Other) const
    {
        return FVector(X - Other.X, Y - Other.Y, Z - Other.Z);
    }

    FVector operator*(double Scale) const
    {
        return FVector(X * Scale, Y * Scale, Z * Scale);
    }

    FVector operator/(double Scale) const
    {
        if (Scale != 0.0)
        {
            double InvScale = 1.0 / Scale;
            return FVector(X * InvScale, Y * InvScale, Z * InvScale);
        }
        return FVector(0.0, 0.0, 0.0);
    }

    FVector& operator+=(const FVector& Other)
    {
        X += Other.X;
        Y += Other.Y;
        Z += Other.Z;
        return *this;
    }

    FVector& operator-=(const FVector& Other)
    {
        X -= Other.X;
        Y -= Other.Y;
        Z -= Other.Z;
        return *this;
    }

    FVector& operator*=(double Scale)
    {
        X *= Scale;
        Y *= Scale;
        Z *= Scale;
        return *this;
    }

    FVector& operator/=(double Scale)
    {
        if (Scale != 0.0)
        {
            double InvScale = 1.0 / Scale;
            X *= InvScale;
            Y *= InvScale;
            Z *= InvScale;
        }
        else
        {
            X = 0.0;
            Y = 0.0;
            Z = 0.0;
        }
        return *this;
    }

    bool operator==(const FVector& Other) const
    {
        return X == Other.X && Y == Other.Y && Z == Other.Z;
    }

    bool operator!=(const FVector& Other) const
    {
        return !(*this == Other);
    }

    double SizeSquared() const
    {
        return X * X + Y * Y + Z * Z;
    }

    double Size() const
    {
        return std::sqrt(SizeSquared());
    }

    //void Normalize()
    //{
    //    double Length = Size();
    //    if (Length != 0.0)
    //    {
    //        double InvLength = 1.0 / Length;
    //        X *= InvLength;
    //        Y *= InvLength;
    //        Z *= InvLength;
    //    }
    //}

    FVector GetNormalized() const
    {
        FVector Result = *this;
        Result.Normalize();
        return Result;
    }

    FVector Normalize() const {
        double Length = std::sqrt(X * X + Y * Y + Z * Z);
        return FVector(X / Length, Y / Length, Z / Length);
    }

};


struct FRotator
{
public:
    double Pitch;
    double Yaw;
    double Roll;

    FRotator() : Pitch(0), Yaw(0), Roll(0) {}
    FRotator(double InPitch, double InYaw, double InRoll) : Pitch(InPitch), Yaw(InYaw), Roll(InRoll) {}

    FRotator operator+(const FRotator& Other) const
    {
        return FRotator(Pitch + Other.Pitch, Yaw + Other.Yaw, Roll + Other.Roll);
    }

    FRotator operator-(const FRotator& Other) const
    {
        return FRotator(Pitch - Other.Pitch, Yaw - Other.Yaw, Roll - Other.Roll);
    }

    FRotator operator*(double Scale) const
    {
        return FRotator(Pitch * Scale, Yaw * Scale, Roll * Scale);
    }

    FRotator operator/(double Scale) const
    {
        if (Scale != 0.0)
        {
            double InvScale = 1.0 / Scale;
            return FRotator(Pitch * InvScale, Yaw * InvScale, Roll * InvScale);
        }
        return FRotator(0.0, 0.0, 0.0);
    }

    FRotator& operator+=(const FRotator& Other)
    {
        Pitch += Other.Pitch;
        Yaw += Other.Yaw;
        Roll += Other.Roll;
        return *this;
    }

    FRotator& operator-=(const FRotator& Other)
    {
        Pitch -= Other.Pitch;
        Yaw -= Other.Yaw;
        Roll -= Other.Roll;
        return *this;
    }

    FRotator& operator*=(double Scale)
    {
        Pitch *= Scale;
        Yaw *= Scale;
        Roll *= Scale;
        return *this;
    }

    FRotator& operator/=(double Scale)
    {
        if (Scale != 0.0)
        {
            double InvScale = 1.0 / Scale;
            Pitch *= InvScale;
            Yaw *= InvScale;
            Roll *= InvScale;
        }
        else
        {
            Pitch = 0.0;
            Yaw = 0.0;
            Roll = 0.0;
        }
        return *this;
    }

    bool operator==(const FRotator& Other) const
    {
        return Pitch == Other.Pitch && Yaw == Other.Yaw && Roll == Other.Roll;
    }

    bool operator!=(const FRotator& Other) const
    {
        return !(*this == Other);
    }

    double SizeSquared() const
    {
        return Pitch * Pitch + Yaw * Yaw + Roll * Roll;
    }

    double Size() const
    {
        return std::sqrt(SizeSquared());
    }

    void Normalize()
    {
        double Length = Size();
        if (Length != 0.0)
        {
            double InvLength = 1.0 / Length;
            Pitch *= InvLength;
            Yaw *= InvLength;
            Roll *= InvLength;
        }
    }

    FRotator GetNormalized() const
    {
        FRotator Result = *this;
        Result.Normalize();
        return Result;
    }
};

struct FVector4
{
    double X;
    double Y;
    double Z;
    double W;

    FVector4() : X(0), Y(0), Z(0), W(0) {}
    FVector4(double InX, double InY, double InZ, double InW) : X(InX), Y(InY), Z(InZ), W(InW) {}


    FVector4 operator+(const FVector4& Other) const
    {
        return FVector4(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
    }

    FVector4 operator-(const FVector4& Other) const
    {
        return FVector4(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
    }

    FVector4 operator*(double Scale) const
    {
        return FVector4(X * Scale, Y * Scale, Z * Scale, W * Scale);
    }

    FVector4 operator/(double Scale) const
    {
        if (Scale != 0.0)
        {
            double InvScale = 1.0 / Scale;
            return FVector4(X * InvScale, Y * InvScale, Z * InvScale, W * InvScale);
        }
        return FVector4(0.0, 0.0, 0.0, 0.0);
    }

    FVector4& operator+=(const FVector4& Other)
    {
        X += Other.X;
        Y += Other.Y;
        Z += Other.Z;
        W += Other.W;
        return *this;
    }

    FVector4& operator-=(const FVector4& Other)
    {
        X -= Other.X;
        Y -= Other.Y;
        Z -= Other.Z;
        W -= Other.W;
        return *this;
    }

    FVector4& operator*=(double Scale)
    {
        X *= Scale;
        Y *= Scale;
        Z *= Scale;
        W *= Scale;
        return *this;
    }

    FVector4& operator/=(double Scale)
    {
        if (Scale != 0.0)
        {
            double InvScale = 1.0 / Scale;
            X *= InvScale;
            Y *= InvScale;
            Z *= InvScale;
            W *= InvScale;
        }
        else
        {
            X = 0.0;
            Y = 0.0;
            Z = 0.0;
            W = 0.0;
        }
        return *this;
    }

    bool operator==(const FVector4& Other) const
    {
        return X == Other.X && Y == Other.Y && Z == Other.Z && W == Other.W;
    }

    bool operator!=(const FVector4& Other) const
    {
        return !(*this == Other);
    }

    double SizeSquared() const
    {
        return X * X + Y * Y + Z * Z + W * W;
    }

    double Size() const
    {
        return std::sqrt(SizeSquared());
    }

    void Normalize()
    {
        double Length = Size();
        if (Length != 0.0)
        {
            double InvLength = 1.0 / Length;
            X *= InvLength;
            Y *= InvLength;
            Z *= InvLength;
            W *= InvLength;
        }
    }

    FVector4 GetNormalized() const
    {
        FVector4 Result = *this;
        Result.Normalize();
        return Result;
    }
};

struct FMatrix
{
public:
    std::array<std::array<double, 4>, 4> M;

    FMatrix()
    {
        M = { { { 0, 0, 0, 0 },
                { 0, 0, 0, 0 },
                { 0, 0, 0, 0 },
                { 0, 0, 0, 0 } } };
    }

    FMatrix(double InM11, double InM12, double InM13, double InM14,
        double InM21, double InM22, double InM23, double InM24,
        double InM31, double InM32, double InM33, double InM34,
        double InM41, double InM42, double InM43, double InM44)
    {
        M = { { { InM11, InM12, InM13, InM14 },
                { InM21, InM22, InM23, InM24 },
                { InM31, InM32, InM33, InM34 },
                { InM41, InM42, InM43, InM44 } } };
    }

    FVector4 TransformFVector4(const FVector4& Vector) const
    {
        FVector4 Result;
        Result.X = Vector.X * M[0][0] + Vector.Y * M[1][0] + Vector.Z * M[2][0] + Vector.W * M[3][0];
        Result.Y = Vector.X * M[0][1] + Vector.Y * M[1][1] + Vector.Z * M[2][1] + Vector.W * M[3][1];
        Result.Z = Vector.X * M[0][2] + Vector.Y * M[1][2] + Vector.Z * M[2][2] + Vector.W * M[3][2];
        Result.W = Vector.X * M[0][3] + Vector.Y * M[1][3] + Vector.Z * M[2][3] + Vector.W * M[3][3];
        return Result;
    }

    // Перегрузки операторов
    FMatrix operator+(const FMatrix& Other) const
    {
        FMatrix Result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Result.M[i][j] = M[i][j] + Other.M[i][j];
            }
        }
        return Result;
    }

    FMatrix operator-(const FMatrix& Other) const
    {
        FMatrix Result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Result.M[i][j] = M[i][j] - Other.M[i][j];
            }
        }
        return Result;
    }

    FMatrix operator*(double Scale) const
    {
        FMatrix Result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Result.M[i][j] = M[i][j] * Scale;
            }
        }
        return Result;
    }

    FMatrix operator*(const FMatrix& Other) const
    {
        FMatrix Result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Result.M[i][j] = 0;
                for (int k = 0; k < 4; k++)
                {
                    Result.M[i][j] += M[i][k] * Other.M[k][j];
                }
            }
        }
        return Result;
    }

    FMatrix operator/(double Scale) const
    {
        if (Scale != 0.0)
        {
            double InvScale = 1.0 / Scale;
            return *this * InvScale;
        }
        return FMatrix();
    }

    FMatrix& operator+=(const FMatrix& Other)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                M[i][j] += Other.M[i][j];
            }
        }
        return *this;
    }

    FMatrix& operator-=(const FMatrix& Other)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                M[i][j] -= Other.M[i][j];
            }
        }
        return *this;
    }

    FMatrix& operator*=(double Scale)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                M[i][j] *= Scale;
            }
        }
        return *this;
    }

    FMatrix& operator*=(const FMatrix& Other)
    {
        *this = *this * Other;
        return *this;
    }

    FMatrix& operator/=(double Scale)
    {
        if (Scale != 0.0)
        {
            double InvScale = 1.0 / Scale;
            *this *= InvScale;
        }
        else
        {
            *this = FMatrix();
        }
        return *this;
    }

    bool operator==(const FMatrix& Other) const
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (M[i][j] != Other.M[i][j])
                    return false;
            }
        }
        return true;
    }

    bool operator!=(const FMatrix& Other) const
    {
        return !(*this == Other);
    }

    // Методы для работы с матрицей

    // Получение транспонированной матрицы
    FMatrix GetTransposed() const
    {
        FMatrix Result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Result.M[i][j] = M[j][i];
            }
        }
        return Result;
    }
};

struct FPlane
{
public:
    double X;
    double Y;
    double Z;
    double W;

    FPlane() : X(0), Y(0), Z(0), W(0) {}
    FPlane(double InX, double InY, double InZ, double InW) : X(InX), Y(InY), Z(InZ), W(InW) {}

    FPlane operator+(const FPlane& Other) const
    {
        return FPlane(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
    }

    FPlane operator-(const FPlane& Other) const
    {
        return FPlane(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
    }

    FPlane operator*(double Scale) const
    {
        return FPlane(X * Scale, Y * Scale, Z * Scale, W * Scale);
    }

    FPlane operator/(double Scale) const
    {
        if (Scale != 0.0)
        {
            double InvScale = 1.0 / Scale;
            return FPlane(X * InvScale, Y * InvScale, Z * InvScale, W * InvScale);
        }
        return FPlane(0.0, 0.0, 0.0, 0.0);
    }

    FPlane& operator+=(const FPlane& Other)
    {
        X += Other.X;
        Y += Other.Y;
        Z += Other.Z;
        W += Other.W;
        return *this;
    }

    FPlane& operator-=(const FPlane& Other)
    {
        X -= Other.X;
        Y -= Other.Y;
        Z -= Other.Z;
        W -= Other.W;
        return *this;
    }

    FPlane& operator*=(double Scale)
    {
        X *= Scale;
        Y *= Scale;
        Z *= Scale;
        W *= Scale;
        return *this;
    }

    FPlane& operator/=(double Scale)
    {
        if (Scale != 0.0)
        {
            double InvScale = 1.0 / Scale;
            X *= InvScale;
            Y *= InvScale;
            Z *= InvScale;
            W *= InvScale;
        }
        else
        {
            X = 0.0;
            Y = 0.0;
            Z = 0.0;
            W = 0.0;
        }
        return *this;
    }

    bool operator==(const FPlane& Other) const
    {
        return X == Other.X && Y == Other.Y && Z == Other.Z && W == Other.W;
    }

    bool operator!=(const FPlane& Other) const
    {
        return !(*this == Other);
    }

    void Normalize()
    {
        double Length = std::sqrt(X * X + Y * Y + Z * Z);
        if (Length != 0.0)
        {
            double InvLength = 1.0 / Length;
            X *= InvLength;
            Y *= InvLength;
            Z *= InvLength;
            W *= InvLength;
        }
    }

    FPlane GetNormalized() const
    {
        FPlane Result = *this;
        Result.Normalize();
        return Result;
    }
};

struct FVector2D
{
public:
    double X;
    double Y;

    FVector2D() : X(0), Y(0) {}
    FVector2D(double InX, double InY) : X(InX), Y(InY) {}

    FVector2D operator+(const FVector2D& Other) const
    {
        return FVector2D(X + Other.X, Y + Other.Y);
    }

    FVector2D operator-(const FVector2D& Other) const
    {
        return FVector2D(X - Other.X, Y - Other.Y);
    }

    FVector2D operator*(double Scale) const
    {
        return FVector2D(X * Scale, Y * Scale);
    }

    FVector2D operator/(double Scale) const
    {
        if (Scale != 0.0)
        {
            double InvScale = 1.0 / Scale;
            return FVector2D(X * InvScale, Y * InvScale);
        }
        return FVector2D(0.0, 0.0);
    }

    FVector2D& operator+=(const FVector2D& Other)
    {
        X += Other.X;
        Y += Other.Y;
        return *this;
    }

    FVector2D& operator-=(const FVector2D& Other)
    {
        X -= Other.X;
        Y -= Other.Y;
        return *this;
    }

    FVector2D& operator*=(double Scale)
    {
        X *= Scale;
        Y *= Scale;
        return *this;
    }

    FVector2D& operator/=(double Scale)
    {
        if (Scale != 0.0)
        {
            double InvScale = 1.0 / Scale;
            X *= InvScale;
            Y *= InvScale;
        }
        else
        {
            X = 0.0;
            Y = 0.0;
        }
        return *this;
    }

    bool operator==(const FVector2D& Other) const
    {
        return X == Other.X && Y == Other.Y;
    }

    bool operator!=(const FVector2D& Other) const
    {
        return !(*this == Other);
    }

    double SizeSquared() const
    {
        return X * X + Y * Y;
    }

    double Size() const
    {
        return std::sqrt(SizeSquared());
    }

    void Normalize()
    {
        double Length = Size();
        if (Length != 0.0)
        {
            double InvLength = 1.0 / Length;
            X *= InvLength;
            Y *= InvLength;
        }
    }

    FVector2D GetNormalized() const
    {
        FVector2D Result = *this;
        Result.Normalize();
        return Result;
    }
};

struct FIntPoint {
    int32_t X; // 0x00(0x04)
    int32_t Y; // 0x04(0x04)
};

struct FIntRect
{
public:
    int32_t Left;
    int32_t Top;

    int32_t Right;
    int32_t Bottom;

    // Конструкторы
    FIntRect() : Left(0), Top(0), Right(0), Bottom(0) {}
    FIntRect(int32_t InLeft, int32_t InTop, int32_t InRight, int32_t InBottom)
        : Left(InLeft), Top(InTop), Right(InRight), Bottom(InBottom) {}

    int32_t Width() const
    {
        return Right - Left;
    }

    int32_t Height() const
    {
        return Bottom - Top;
    }

    bool IsEmpty() const
    {
        return (Width() <= 0) || (Height() <= 0);
    }
};

struct FQuat
{
    double X;
    double Y;
    double Z;
    double W;

    FQuat() : X(0.0), Y(0.0), Z(0.0), W(1.0) {}
    FQuat(double InX, double InY, double InZ, double InW) : X(InX), Y(InY), Z(InZ), W(InW) {}

    FVector RotateVectorByQuaternion(const FVector& VectorToRotate) const
    {
        FQuat V(VectorToRotate.X, VectorToRotate.Y, VectorToRotate.Z, 0.0);

        FQuat V_prime = (*this) * V * Inverse();

        return FVector(V_prime.X, V_prime.Y, V_prime.Z);
    }

    FQuat Inverse() const
    {
        double Norm = X * X + Y * Y + Z * Z + W * W;
        return FQuat(-X / Norm, -Y / Norm, -Z / Norm, W / Norm);
    }

    FQuat operator*(const FQuat& Other) const
    {
        return FQuat(0.0, 0.0, 0.0, 1.0);
    }
};
//struct FSDA {
//    FQuat  Rotation; //20
//    FVector Translation; //18
//    char l[8];  //8
//    //FVector Scale; //18
//}
struct FTransform {
    FQuat  Rotation; //20
    FVector Translation; //18
    char l[8];  //8
    FVector Scale; //18
    char e[8];  //8

    FTransform() {}
    FTransform(const FQuat& InRotation, const FVector& InTranslation, const FVector& InScale3D)
        : Rotation(InRotation), Translation(InTranslation), Scale(InScale3D) {}

    D3DMATRIX to_matrix()
    {
        D3DMATRIX m;
        m._41 = Translation.X;
        m._42 = Translation.Y;
        m._43 = Translation.Z;

        double x2 = Rotation.X + Rotation.X;
        double y2 = Rotation.Y + Rotation.Y;
        double z2 = Rotation.Z + Rotation.Z;

        double xx2 = Rotation.X * x2;
        double yy2 = Rotation.Y * y2;
        double zz2 = Rotation.Z * z2;
        m._11 = (1.0f - (yy2 + zz2)) * Scale.X;
        m._22 = (1.0f - (xx2 + zz2)) * Scale.Y;
        m._33 = (1.0f - (xx2 + yy2)) * Scale.Z;

        double yz2 = Rotation.Y * z2;
        double wx2 = Rotation.W * x2;
        m._32 = (yz2 - wx2) * Scale.Z;
        m._23 = (yz2 + wx2) * Scale.Y;

        double xy2 = Rotation.X * y2;
        double wz2 = Rotation.W * z2;
        m._21 = (xy2 - wz2) * Scale.Y;
        m._12 = (xy2 + wz2) * Scale.X;

        double xz2 = Rotation.X * z2;
        double wy2 = Rotation.W * y2;
        m._31 = (xz2 + wy2) * Scale.Z;
        m._13 = (xz2 - wy2) * Scale.X;

        m._14 = 0.0f;
        m._24 = 0.0f;
        m._34 = 0.0f;
        m._44 = 1.0f;

        return m;
    }
};

D3DMATRIX matrix_multiplication(D3DMATRIX pM1, D3DMATRIX pM2) {
    D3DMATRIX out;
    out._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
    out._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
    out._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
    out._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
    out._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
    out._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
    out._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
    out._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
    out._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
    out._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
    out._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
    out._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
    out._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
    out._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
    out._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
    out._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

    return out;
}

double CalculateDistance(const FVector& Vec1, const FVector& Vec2)
{
    double deltaX = Vec2.X - Vec1.X;
    double deltaY = Vec2.Y - Vec1.Y;
    double deltaZ = Vec2.Z - Vec1.Z;

    return std::sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}


D3DMATRIX matrix(FVector Rotation) {
    float radPitch = (Rotation.X * double(M_PI) / 180.f);
    float radYaw = (Rotation.Y * double(M_PI) / 180.f);
    float radRoll = (Rotation.Z * double(M_PI) / 180.f);

    float SP = sinf(radPitch);
    float CP = cosf(radPitch);
    float SY = sinf(radYaw);
    float CY = cosf(radYaw);
    float SR = sinf(radRoll);
    float CR = cosf(radRoll);

    D3DMATRIX matrix;
    matrix.m[0][0] = CP * CY;
    matrix.m[0][1] = CP * SY;
    matrix.m[0][2] = SP;
    matrix.m[0][3] = 0.f;

    matrix.m[1][0] = SR * SP * CY - CR * SY;
    matrix.m[1][1] = SR * SP * SY + CR * CY;
    matrix.m[1][2] = -SR * CP;
    matrix.m[1][3] = 0.f;

    matrix.m[2][0] = -(CR * SP * CY + SR * SY);
    matrix.m[2][1] = CY * SR - CR * SP * SY;
    matrix.m[2][2] = CR * CP;
    matrix.m[2][3] = 0.f;

    matrix.m[3][0] = 0;
    matrix.m[3][1] = 0;
    matrix.m[3][2] = 0;
    matrix.m[3][3] = 1.f;

    return matrix;
}

struct FWorldCachedViewInfo {
    FMatrix ViewMatrix;
    FMatrix ProjectionMatrix;
    FMatrix ViewProjectionMatrix;
    FMatrix ViewToWorld;
};

struct BonesArray {
    uintptr_t Array1;
    int Size1;
    uintptr_t Array2;
    int Size2;
};

enum class EFortItemTier : uint8_t {
    No_Tier = 0,
    I = 1,
    II = 2,
    III = 3,
    IV = 4,
    V = 5,
    VI = 6,
    VII = 7,
    VIII = 8,
    IX = 9,
    X = 10,
    NumItemTierValues = 11,
    EFortItemTier_MAX = 12
};

enum class EFortRarity : uint8_t {
    Common = 0,
    Uncommon = 1,
    Rare = 2,
    Epic = 3,
    Legendary = 4,
    Mythic = 5,
    Transcendent = 6,
    Unattainable = 7,
    NumRarityValues = 8,
    EFortRarity_MAX = 9
};

struct GameInfoStruct {
    uintptr_t UWorld;
    uintptr_t MatrixPointer;
    FWorldCachedViewInfo ViewInfo;
    uintptr_t UGameInstance;
    uintptr_t AGameState;
    uintptr_t ULocalPlayer;
    uintptr_t PlayerCameraManager;
    uintptr_t CameraRoot;
    uintptr_t AController;
    uintptr_t LocalPawn;
    uintptr_t URoot;
    uintptr_t ULocalMesh;
    uintptr_t ULocalMovement;
    uintptr_t APlayerState;
    uintptr_t UEngine;
    uintptr_t Texture;
    uintptr_t AFortWeapon;
    uintptr_t WeaponData;
    uintptr_t DisplayName;
    uintptr_t GameUserSettings;
    uintptr_t UPlayerInput;
    FIntRect ViewRect;
    int MyTeamId;
    uintptr_t TPlayerArray;
    int PlayerArrayCount;
    float LastDistance;
    uint8_t IsRanged;
    float Fov;
    uint8_t ItemType;
};


struct DWORD64TArray {
    uintptr_t Array;
    int ArrayCount;
};

struct WeaponDataDefinition {
    uintptr_t WeaponData;
    uintptr_t DisplayName;
    DWORD64TArray FString;
    std::string Name;
    EFortItemTier Rarity;
    EFortRarity PawnWeapon;
};

struct RenderTimes {
    float LastSubmitTime = 0;
    float LastRenderTime = 0;
    float LastRenderTimeOnScreen = 0;
};

struct FBone {
    std::string Name = "";
    bool bIsActivated = false;
    int Bone = 0;
    FTransform Transform;

    FBone(const std::string& InName, bool InActivated, int InBone)
        : Name(InName), bIsActivated(InActivated), Bone(InBone) {
    }

    FBone(const std::string& InName, bool InActivated, int InBone, const FTransform& InTransform)
        : Name(InName), bIsActivated(InActivated), Bone(InBone), Transform(InTransform) {
    }

    FBone()
    {
    }
};

struct AObject {
    int64_t Type = 0; //0 = undefined, // 1 = players // 2 = items // 3 = entity
    FVector RootLocation;
    uintptr_t AActor;
    uintptr_t USceneComponent;
    virtual ~AObject() {}

};

struct AFortWeapon : AObject {
    WeaponDataDefinition* WeaponData = nullptr;
};

struct APawn : AObject {
    std::string Name = "";
    uintptr_t UMesh;
    AFortWeapon* CurrentWeapon = nullptr;

};



std::vector<AObject*> Objects;


APawn* GetAPawnByName(std::string Name)
{
    for (int64_t i = 0; i < Objects.size(); i++)
    {
        if (Objects[i])
        {
            if (Objects[i]->Type == 1)
            {
                APawn* Pawn = dynamic_cast<APawn*>(Objects[i]);
                if (Name == Pawn->Name)
                {
                    return Pawn;
                }
            }
        }
    }
    return nullptr;
}


struct DefaultPawn {
    uintptr_t AActor;
    uintptr_t USceneComponent;
    uintptr_t UMovement;
    uintptr_t APlayerState;
    uintptr_t UMesh;
    WeaponDataDefinition* Weapon = nullptr;
    uintptr_t AFortWeapon;
    uintptr_t PreviosWeapon;
    uintptr_t BoneArray;
    int BoneArraySize;
    bool bIsBot;
    bool bIsDead = false;
    bool bIsDBNO = false;
    bool bIsVisible;
    std::string Name;
    int TeamIndex;
    FVector HeadLocation;
    FVector ZeroLocation;
    FTransform FHeadLocation;
    FTransform FZeroLocation;
    FVector2D HeadLocationW2S;
    FVector2D ZeroLocationW2S;
    std::string Platform;
    int Kills;
    FVector GlobalAimBonePos = { };
    FVector2D GlobalAimBonePosW2S = { };
    double Distance;
    std::vector<FBone*> Bones;
    FBone* DefaultBone = new FBone;
    std::vector<FTransform> FTBones;
    RenderTimes RenderData;

    FBone* GetBoneByName(std::string BoneName)
    {
        for (int64_t i = 0; i < Bones.size(); i++)
        {
            if (BoneName == Bones[i]->Name)
            {
                return Bones[i];
            }
        }
        return DefaultBone;
    }

};



struct PawnDistance {
    DefaultPawn* pawn;
    double distanceMeters;

    // Конструктор для удобства создания объектов PawnDistance
    PawnDistance(DefaultPawn* p, double dist) : pawn(p), distanceMeters(dist) {}

    // Функция для сравнения объектов PawnDistance по значению distanceMeters (от большего к меньшему)
    static bool CompareByDistance(const PawnDistance& a, const PawnDistance& b) {
        return a.distanceMeters > b.distanceMeters; // Сравниваем по убыванию
    }
};



struct TMeshBone {
    uintptr_t TboneArray1;
    uintptr_t TboneArraySize1;
    uintptr_t TboneArray2;
};

struct TFtrAndVis {
    FTransform Zero_Loc;

};



namespace Offsets {
    uintptr_t PlayerNameEncrypted = 0;
    uintptr_t UWorld = 0;
    uintptr_t MatrixPointer = 0;
    uintptr_t ViewInfo = 0x0;
    uintptr_t UGameInstance = 0;
    uintptr_t AGameState = 0;
    uintptr_t TLocalPlayers = 0;
    uintptr_t AController = 0x30;
    uintptr_t Pawn = 0x338;
    uintptr_t PawnPrivate = 0x308;
    uintptr_t URoot = 0x198;
    uintptr_t UMesh = 0x318;
    uintptr_t UMovement = 0x320;
    uintptr_t APlayerStateFromController = 0x298;
    uintptr_t APlayerStateFromPawn = 0x2b0;
    uintptr_t UEngine = 0x20;
    uintptr_t Texture = 0x328;
    uintptr_t AFortWeapon = 0xa68;
    uintptr_t WeaponData = 0;
    uintptr_t GameUserSettings = 0x228;
    uintptr_t UPlayerInput = 0x408;
    uintptr_t ViewRect = 0x80;
    uintptr_t TeamIndex = 0;
    uintptr_t KillScore = 0;
    uintptr_t Aim = 0x0;
    uintptr_t TPlayerArray = 0x2a8;
    uintptr_t PlayerArrayCount = 0x2b0;
    uintptr_t TBoneArray = 0x5b0;
    uintptr_t TargetedFortPawn = 0;
    uintptr_t ComponentToWorld = 0x1c0;
    uintptr_t RenderTimes = 0; // 0x2d4 + 0x14
}


struct AActor {
    uintptr_t Actor;
    uintptr_t URoot;
};



struct ByteColor {
    uint8_t R, G, B, A;
};

#include <limits>
#include <cstdint>

std::unordered_map<uint8_t, std::string> WeaponType = {
    {1, "ShotGun"},
    {17, "MachineGun"},
    {25, "Sniper"},
};


std::unordered_map<EFortItemTier, ByteColor> rarityColors = {
    {EFortItemTier::No_Tier, {190, 190, 190, 255}},
    {EFortItemTier::I, {190, 190, 190, 255}},
    {EFortItemTier::II, {0, 255, 0, 255}},
    {EFortItemTier::III, {20, 20, 255, 255}},
    {EFortItemTier::IV, {128, 0, 128, 255}},
    {EFortItemTier::V, {255, 165, 0, 255}},
    {EFortItemTier::VI, {255, 223, 186, 255}},
    {EFortItemTier::VII, {0, 252, 239, 255}},
};

std::unordered_map<EFortRarity, ByteColor> rarityAWeaponColors = {
    {EFortRarity::Common, {190, 190, 190, 255}},
    {EFortRarity::Uncommon, {0, 255, 0, 255}},
    {EFortRarity::Rare, {20, 20, 255, 255}},
    {EFortRarity::Epic, {128, 0, 128, 255}},
    {EFortRarity::Legendary, {255, 165, 0, 255}},
    {EFortRarity::Mythic, {255, 223, 186, 255}},
    {EFortRarity::Transcendent, {0, 252, 239, 255}},
};


struct AItemActor {
    uintptr_t AActor;
    uintptr_t URoot;
    FVector Location;
    FVector2D LocationW2S;
    WeaponDataDefinition* WeaponData;
};

AItemActor GetItemInfo(AItemActor Item)
{
    Item.AActor = Item.AActor;
    Item.URoot = Item.URoot;
}

struct PlayerArray
{
    uintptr_t Array;
    int ArrayCount;
};

struct InputArray
{
    uintptr_t Array;
    int ArrayCount;
};

struct rew {
    std::vector<uintptr_t> data;
};

struct  PawnDeadStatusStruct {
	char pad_758_0 : 1; // 0x758(0x01)
	char bUseBaseChanged : 1; // 0x758(0x01)
	char bIgnoreNextFallingDamage : 1; // 0x758(0x01)
	char bAllowDeathFromFallingDamage : 1; // 0x758(0x01)
	char bIsDying : 1; // 0x758(0x01)
	char bPlayedDying : 1; // 0x758(0x01)
	char bIsHiddenForDeath : 1; // 0x758(0x01)
	char bIsKnockedback : 1; // 0x758(0x01)
};


struct  PawnDBNOStatusStruct {
    char bTreatAsPawnForHitMarkers : 1; // 0x982(0x01)
    char bDisplayPawnHitMarkersForChildActors : 1; // 0x982(0x01)
    char bUsesStats : 1; // 0x982(0x01)
    char bAllowBuildingActorTeleport : 1; // 0x982(0x01)
    char bIsDBNO : 1; // 0x982(0x01)
    char bWasDBNOOnDeath : 1; // 0x982(0x01)
    char bCachedIsInAthena : 1; // 0x982(0x01)
    char bShouldUseCharacterMovementIdleFastPath : 1; // 0x982(0x01)
};
//template <typename T>
//struct ReadResult {
//    std::vector<T> data;
//};