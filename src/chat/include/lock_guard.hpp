namespace ychat
{
	template<class T>
	class lock_guard
	{
	public:
		lock_guard (T &t)
			:t_(t)
		{
			t.lock ();
		}
		~lock_guard()
		{
			t_.unlock ();
		}
	private:
		T &t_;
	};
}