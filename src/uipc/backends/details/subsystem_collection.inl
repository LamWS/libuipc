namespace uipc::backend
{
template <std::derived_from<ISimSystem> T>
void SubsystemCollection<T>::register_subsystem(T* subsystem)
{
    UIPC_ASSERT(subsystem->is_building(),
                "`register_subsystem()` can only be called when the SimEngine builds system.");
    if(subsystem->is_valid())
        m_subsystem_buffer.push_back(subsystem);
}

template <std::derived_from<ISimSystem> T>
void SubsystemCollection<T>::build()
{
    std::erase_if(m_subsystem_buffer,
                  [](T* subsystem) { return !subsystem->is_valid(); });

    m_subsystems.reserve(m_subsystem_buffer.size());
    std::ranges::move(m_subsystem_buffer, std::back_inserter(m_subsystems));
    m_subsystem_buffer.clear();
    built = true;
}

template <std::derived_from<ISimSystem> T>
span<T*> SubsystemCollection<T>::view() noexcept
{
    check_build();
    return m_subsystems;
}

template <std::derived_from<ISimSystem> T>
span<T* const> SubsystemCollection<T>::view() const noexcept
{
    check_build();
    return m_subsystems;
}

template <std::derived_from<ISimSystem> T>
inline void SubsystemCollection<T>::check_build() const noexcept
{
    UIPC_ASSERT(built, "SubsystemCollection is not built yet. Call build() before accessing the subsystems");
}
}  // namespace uipc::backend