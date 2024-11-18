export module foundation.Vector2D;

import foundation.basetypes;

namespace mbg {
	export template<typename T>
		class Vector2D {
		public:
			Vector2D(T x, T y) : x_(x), y_(y) {}

			T getX() const { return x_; }
			T getY() const { return y_; }
			void setX(T x) { x_ = x; }
			void setY(T y) { y_ = y; }

			Double length() const { return sqrt(x_ * x_ + y_ * y_); }

			Vector2D<T>  operator+(const Vector2D<T>& v2) const { return Vector2D<T>(x_ + v2.x_, y_ + v2.y_); }
			Vector2D<T>  operator-(const Vector2D<T>& v2) const { return Vector2D<T>(x_ - v2.x_, y_ - v2.y_); }
			Vector2D<T>  operator*(T scalar) const { return Vector2D<T>(x_ * scalar, y_ * scalar); }
			Vector2D<T>  operator/(T scalar) const { return Vector2D<T>(x_ / scalar, y_ / scalar); }

			Vector2D<T>& operator+=(Vector2D<T> v) {
				x_ += v.x_;
				y_ += v.y_;
				return *this;
			}
			Vector2D<T>& operator-=(Vector2D<T> v) {
				x_ -= v.x_;
				y_ -= v.y_;
				return *this;
			}
			Vector2D<T>& operator*=(T scalar) {
				x_ *= scalar;
				y_ *= scalar;
				return *this;
			}
			Vector2D<T>& operator/=(T scalar) {
				x_ /= scalar;
				y_ /= scalar;
				return *this;
			}

			//void normalize();

		private:
			T x_;
			T y_;
	};

	export using Position = Vector2D<Integer>;
	export using Index = Vector2D<Integer>;

	/*
	template<typename T> void Vector2D<T>::normalize() {
		Float l = length();
		if (l > 0) // we never want to attempt to divide by 0
		{
			(*this) /= l;
		}
	}*/
}