# frozen_string_literal: true

RSpec.describe AdaUrl::Url do
  it "has a version number" do
    expect(AdaUrl::VERSION).not_to be nil
  end

  shared_examples "a valid url" do
    it "knows the url is valid" do
      expect(subject.valid?).to eq(true)
      expect { subject.validate! }.not_to raise_error
    end
  end

  shared_examples "a common host type" do
    it "has a host type of common" do
      expect(subject.host_type).to eq(:common)
    end

    it "getters all return correct values" do
      expect(subject.common_host_type?).to eq(true)
      expect(subject.ipv4?).to eq(false)
      expect(subject.ipv6?).to eq(false)
    end
  end

  shared_examples "an ipv4 host type" do
    it "has a host type of ipv4" do
      expect(subject.host_type).to eq(:ipv4)
    end

    it "getters all return correct values" do
      expect(subject.common_host_type?).to eq(false)
      expect(subject.ipv4?).to eq(true)
      expect(subject.ipv6?).to eq(false)
    end
  end

  shared_examples "an ipv6 host type" do
    it "has a host type of ipv6" do
      expect(subject.host_type).to eq(:ipv6)
    end

    it "getters all return correct values" do
      expect(subject.common_host_type?).to eq(false)
      expect(subject.ipv4?).to eq(false)
      expect(subject.ipv6?).to eq(true)
    end
  end

  context "url smoke tests" do
    subject { AdaUrl::Url.new(url) }

    context "google.com" do
      let(:url) { "https://google.com" }

      it_behaves_like "a valid url"
      it_behaves_like "a common host type"

      it "is google" do
        expect(subject.host).to eq("google.com")
        expect(subject.protocol).to eq("https:")
      end
    end

    context "with auth" do
      let(:url) { "postgresql://reece:foobar@login.com" }

      it_behaves_like "a valid url"
      it_behaves_like "a common host type"

      it "has a username" do
        expect(subject.username).to eq("reece")
      end

      it "has a password" do
        expect(subject.password).to eq("foobar")
      end
    end

    context "with search params" do
      let(:url) { "https://google.com/search?q=ruby" }

      it "has search params" do
        expect(subject.search).to eq("?q=ruby")
      end

      it_behaves_like "a valid url"
      it_behaves_like "a common host type"
    end
  end

  # it "can parse a basic url" do
  #   url = AdaUrl::Url.new('https://google.com')
  #
  #   expect(url.host).to eq('google.com')
  #   expect(url.protocol).to eq('https:')
  # end

  # it "can parse a url with authentication and a path" do
  #   url = AdaUrl::Url.new('https://reece:foobar@example.com/hello')
  #
  #   expect(url.host).to eq('example.com')
  #   expect(url.password).to eq('foobar')
  #   expect(url.username).to eq('reece')
  #   expect(url.valid?).to eq(true)
  #   expect(url.host_type).to eq(:common)
  #   expect(url.common_host_type?).to eq(true)
  # end
end
