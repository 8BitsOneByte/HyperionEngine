#pragma once

namespace Hyperion
{
    /// Represents the elapsed time between two frames (in seconds).
    /// Implicitly converts to float so it can be used directly in arithmetic.
    class Timestep
    {
    public:
        explicit Timestep(float time = 0.0f)
            : m_Time(time)
        {
        }

        /// Implicit conversion to seconds as a plain float.
        operator float() const { return m_Time; }

        float GetSeconds() const { return m_Time; }
        float GetMilliseconds() const { return m_Time * 1000.0f; }

    private:
        float m_Time;
    };
} // namespace Hyperion
