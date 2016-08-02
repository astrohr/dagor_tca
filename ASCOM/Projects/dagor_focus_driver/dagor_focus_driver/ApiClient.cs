using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net.Http;
using System.Net.Http.Headers;
using System.Diagnostics;
using Newtonsoft.Json;

namespace ASCOM.DagorApiClient
{

    public interface IRepr
    {
    }

    public interface IReprReady : IRepr
    {
        bool ready { get; set; }
    }
    

    public interface IApiClient<ReadyRepr>
        where ReadyRepr : class
    {
        int Retries { get; set; }
        bool IsReady { get; }
        ReadyRepr ApiGetReady();
    }


    class CommError : Exception
    {
        public CommError(string message) : base(message)
        { }
    }

    public abstract class ApiClient<ReadyRepr> : IApiClient<ReadyRepr>
        where ReadyRepr: class
    {

        public ApiClient(string Proto, string Server, int Port)
        {
            this.Proto = Proto;
            this.Server = Server;
            this.Port = Port;
        }

        protected abstract string _DevPath();

        protected string Proto { get; set; }
        protected string Server { get; set; }
        protected int Port { get; set; }
        protected string DevPath {
            get
            {
                string devPath = _DevPath();
                if (devPath.EndsWith("/"))
                    devPath = devPath.Substring(0, devPath.Length - 1);
                return devPath;
            }
        }

        private HttpClient _client;
        protected HttpClient client
        {
            get {
                if (_client == null) {
                    _client = new HttpClient();
                    _client.BaseAddress = new Uri(Proto + "://" + Server + ":" + Port + "/" + DevPath + "/");
                    // Add an Accept header for JSON format:
                    client.DefaultRequestHeaders.Accept.Add(
                        new MediaTypeWithQualityHeaderValue("application/json"));
                }
                return _client;
            }
        }


        public int Retries {
            get { return 5; }
            set { throw new NotImplementedException(); }
        }

        public bool IsReady
        {
            get
            {
                try
                {
                    ReadyRepr ready_repr = ExecuteGET<ReadyRepr>("");
                    return ((IReprReady)ready_repr).ready;
                }
                catch (Exception ex)
                {
                    // TODO Log exception
                    Debug.WriteLine(ex.ToString());
                    return false;
                } 


            }
        }


        ReadyRepr IApiClient<ReadyRepr>.ApiGetReady()
        {
            throw new System.NotImplementedException();
        }


        protected ResponseRepr ExecuteGET<ResponseRepr>(string url)
        {
            int retries = Retries;

            if (!string.IsNullOrEmpty(url) && url.EndsWith("/"))
                url += "/";

            while (retries-- > 0)
            {
                // List data response.
                HttpResponseMessage response = client.GetAsync(url).Result;  // Blocking call!

                if (response.IsSuccessStatusCode)
                {
                    // Parse the response body. Blocking!
                    var content = response.Content.ReadAsStringAsync().Result;
                    Debug.WriteLine(content);
                    var state = JsonConvert.DeserializeObject<ResponseRepr>(content);
                    return state;
                }
            }

            throw new CommError("Retries exhausted");

        }

    }



    #region DefaultImplementations

    // Some default implementations that might be useful

    public class DefaultReadyRepr : IReprReady
    {
        public bool ready { get; set; }
    }


    #endregion

}
