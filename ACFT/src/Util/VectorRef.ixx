export module ACFT.VectorRef;

import <vector>;
import <functional>;
import <expected>;
import <exception>;

namespace ACFT
{
	export template<typename T, typename Tag>
	using VerifyFunction = std::function<bool(const T&, const Tag&)>;
	
	export enum class VectorRefException
	{
		VectorInvalid, IndexExceeded, ObjectInconsistent
	};
	
	/**
	 * A reference to a specific element in a vector, has the ability to check if the element is moved or deleted.
	 * However, the vector's pointer must always be valid.
	 * Meanwhile, if the ref became invalid, manual handling is required.
	 * 
	 * @tparam T The element type.
	 * @tparam Tag The tag type, used to decide whether the ref is valid.
	 */
	export template<typename T, typename Tag>
	class VectorRef
	{
	public:
		VectorRef() = default;

		VectorRef(std::vector<T>* vec, size_t idx, const Tag& verify_tag, const VerifyFunction<T, Tag> &verify_pred)
			: p_Vector(vec), m_Idx(idx), m_Tag(verify_tag), m_Verifier(verify_pred)
		{ }

		VectorRef(std::vector<T>* vec, size_t idx)
			: p_Vector(vec), m_Idx(idx)
		{ }

		VectorRef(const VectorRef &other) = default;
		VectorRef(VectorRef &&other) noexcept = default;

		VectorRef& operator=(const VectorRef &other)
		{
			p_Vector = other.p_Vector;
			m_Verifier = other.m_Verifier;
			m_Idx = other.m_Idx;
			m_Tag = other.m_Tag;
		}

		VectorRef& operator=(VectorRef &&other)
		{
			p_Vector = other.p_Vector;
			m_Verifier = std::move(other.m_Verifier);
			m_Idx = other.m_Idx;
			m_Tag = std::move(other.m_Tag);
		}

		bool operator==(const VectorRef &other)
		{
			if (m_Verifier ^ other.m_Verifier)
				return false;

			if (m_Verifier)
				return p_Vector == other.p_Vector && m_Idx == other.m_Idx && m_Tag == other.m_Tag;
			else
				return p_Vector == other.p_Vector && m_Idx == other.m_Idx;
		}

		/**
		 * Access the element WITHOUT checking, this may throw an exception.
		 */
		T& operator*()
		{
			if (!p_Vector)
				throw std::exception("VectorRef: Pointer to the vector is nullptr!");

			return p_Vector->at(m_Idx);
		}

		/**
		 * Get the pointer to the element WITHOUT checking, this may throwan exception.
		 */
		T* operator->()
		{
			if (!p_Vector)
				throw std::exception("VectorRef: Pointer to the vector is nullptr!");
			
			return &p_Vector->at(m_Idx);
		}

		std::expected<T*, VectorRefException> Access() noexcept
		{
			if (!p_Vector)
				return VectorRefException::VectorInvalid;

			if (m_Idx >= p_Vector->size())
				return VectorRefException::IndexExceeded;

			if (m_Verifier && !m_Verifier(p_Vector->at(m_Idx), m_Tag))
				return VectorRefException::ObjectInconsistent;

			return &p_Vector->at(m_Idx);
		}

		void SetVectorPointer(std::vector<T>* p)
		{
			p_Vector = p;
		}

		/**
		 * Set the tag used to verify validation. Tag is not used if VerifyFunction is not passed during construction.
		 * 
		 * \param tag
		 */
		void SetTag(const Tag &tag)
		{
			m_Tag = tag;
		}

		void SetIndex(size_t index)
		{
			m_Idx = index;
		}

	private:
		std::vector<T> *p_Vector = nullptr;
		VerifyFunction<T, Tag> m_Verifier;
		Tag m_Tag;
		size_t m_Idx = 0;
	};
}
