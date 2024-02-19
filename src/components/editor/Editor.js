import SpriteViewer from "./SpriteViewer";
import Map from "./map/Map";

export default function Editor(
  { fileProcessed }: { fileProcessed: boolean }
) {
  return (
    <div>
      <SpriteViewer
        fileProcessed={fileProcessed}
      />
      <Map
        fileProcessed={fileProcessed}
      />
    </div>
  );
}
