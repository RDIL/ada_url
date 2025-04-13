class AdaUrl::Url
  def common_host_type? = host_type == :common
  def ipv4? = host_type == :ipv4
  def ipv6? = host_type == :ipv6

  def validate!
    raise "Invalid URL" unless self.valid?
  end
end
